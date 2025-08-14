#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Frontend/FrontendActions.h"
#include "llvm/Support/CommandLine.h"
#include "DCCConsumer.h"

using namespace clang;
using namespace clang::tooling;

class DCCAction : public ASTFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance& CI, StringRef) override {
        return std::make_unique<DCCConsumer>(CI.getASTContext());
    }
};

static llvm::cl::OptionCategory Cat("dcc options");

int main(int argc, const char** argv) {
    auto EP = CommonOptionsParser::create(argc, argv, Cat);
    if (!EP) {
        llvm::errs() << EP.takeError();
        return 1;
    }
    CommonOptionsParser& OP = *EP;
    ClangTool Tool(OP.getCompilations(), OP.getSourcePathList());
    return Tool.run(newFrontendActionFactory<DCCAction>().get());
}
