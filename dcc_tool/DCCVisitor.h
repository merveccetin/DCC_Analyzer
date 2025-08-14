#ifndef DCCVISITOR_H
#define DCCVISITOR_H

#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "llvm/ADT/DenseMap.h"
#include <string>

struct ParamInfo {
    bool isControl = false;
    int dataUses = 0;
    int guardedUses = 0;
};

class DCCVisitor : public clang::RecursiveASTVisitor<DCCVisitor> {
public:
    explicit DCCVisitor(clang::ASTContext &C);

    bool shouldVisitTemplateInstantiations() const { return true; }
    bool TraverseFunctionDecl(clang::FunctionDecl* FD);
    bool TraverseIfStmt(clang::IfStmt* S);
    bool TraverseWhileStmt(clang::WhileStmt* S);
    bool TraverseDoStmt(clang::DoStmt* S);
    bool TraverseForStmt(clang::ForStmt* S);
    bool TraverseSwitchStmt(clang::SwitchStmt* S);
    bool TraverseConditionalOperator(clang::ConditionalOperator* CO);
    bool VisitDeclRefExpr(clang::DeclRefExpr* E);

private:
    clang::ASTContext& Ctx;
    int depth = 0;
    bool inCond = false;
    llvm::DenseMap<const clang::ParmVarDecl*, ParamInfo> info;

    void reset(clang::FunctionDecl* FD);
    void mark(clang::Stmt* S);
    static std::string loc(const clang::SourceManager& SM, clang::SourceLocation L);
    void emit(const clang::FunctionDecl* FD);
};

#endif // DCCVISITOR_H
