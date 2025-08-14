#include "DCCVisitor.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FormatVariadic.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/SourceLocation.h"

using namespace clang;

DCCVisitor::DCCVisitor(ASTContext &C) : Ctx(C) {}

bool DCCVisitor::TraverseFunctionDecl(FunctionDecl* FD) {
    if(!FD || !FD->isThisDeclarationADefinition()) return true;
    const auto& SM = Ctx.getSourceManager();
    if(!SM.isInMainFile(SM.getSpellingLoc(FD->getLocation()))) return true;
    reset(FD);
    RecursiveASTVisitor::TraverseFunctionDecl(FD);
    emit(FD);
    return true;
}

bool DCCVisitor::TraverseIfStmt(IfStmt* S) {
    mark(S->getCond());
    depth++;
    if(auto* T = S->getThen()) TraverseStmt(T);
    depth--;
    if(auto* E = S->getElse()) { depth++; TraverseStmt(E); depth--; }
    return true;
}

bool DCCVisitor::TraverseWhileStmt(WhileStmt* S) {
    mark(S->getCond());
    depth++;
    TraverseStmt(S->getBody());
    depth--;
    return true;
}

bool DCCVisitor::TraverseDoStmt(DoStmt* S) {
    depth++;
    TraverseStmt(S->getBody());
    depth--;
    mark(S->getCond());
    return true;
}

bool DCCVisitor::TraverseForStmt(ForStmt* S) {
    if(S->getInit()) TraverseStmt(S->getInit());
    mark(S->getCond());
    if(S->getInc()) TraverseStmt(S->getInc());
    depth++;
    TraverseStmt(S->getBody());
    depth--;
    return true;
}

bool DCCVisitor::TraverseSwitchStmt(SwitchStmt* S) {
    mark(S->getCond());
    depth++;
    TraverseStmt(S->getBody());
    depth--;
    return true;
}

bool DCCVisitor::TraverseConditionalOperator(ConditionalOperator* CO) {
    mark(CO->getCond());
    depth++;
    TraverseStmt(CO->getTrueExpr());
    TraverseStmt(CO->getFalseExpr());
    depth--;
    return true;
}

bool DCCVisitor::VisitDeclRefExpr(DeclRefExpr* E) {
    if(auto* P = llvm::dyn_cast_or_null<ParmVarDecl>(E->getDecl())) {
        auto it = info.find(P);
        if(it == info.end()) return true;
        if(inCond) it->second.isControl = true;
        else {
            it->second.dataUses++;
            if(depth > 0) it->second.guardedUses++;
        }
    }
    return true;
}

void DCCVisitor::reset(FunctionDecl* FD) {
    depth = 0;
    inCond = false;
    info.clear();
    for(auto* p: FD->parameters()) info[p] = ParamInfo{};
}

void DCCVisitor::mark(Stmt* S) {
    if(!S) return;
    bool prev = inCond;
    inCond = true;
    TraverseStmt(S);
    inCond = prev;
}

std::string DCCVisitor::loc(const SourceManager& SM, SourceLocation L) {
    PresumedLoc P = SM.getPresumedLoc(L);
    if(!P.isValid()) return "";
    return std::string(P.getFilename()) + ":" + std::to_string(P.getLine());
}

void DCCVisitor::emit(const FunctionDecl* FD) {
    const auto& SM = Ctx.getSourceManager();
    int all = 0, guard = 0;
    bool hasCtrl = false;
    for(auto& kv: info) {
        all += kv.second.dataUses;
        guard += kv.second.guardedUses;
        hasCtrl |= kv.second.isControl;
    }
    double dcc = all ? double(guard) / double(all) : 0.0;
    static bool hdr = false;
    if(!hdr) {
        llvm::outs() << "file,function,has_control,all_data_uses,guarded_data_uses,dDCC\n";
        hdr = true;
    }
    llvm::outs() << loc(SM, FD->getBeginLoc()) << ","
                 << FD->getNameAsString() << ","
                 << (hasCtrl ? "true" : "false") << ","
                 << all << "," << guard << ","
                 << llvm::formatv("{0:F3}", dcc) << "\n";
}
