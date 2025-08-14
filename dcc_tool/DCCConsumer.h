#ifndef DCCCONSUMER_H
#define DCCCONSUMER_H

#include "clang/AST/ASTConsumer.h"
#include "DCCVisitor.h"

class DCCConsumer : public clang::ASTConsumer {
public:
    explicit DCCConsumer(clang::ASTContext &C);
    void HandleTranslationUnit(clang::ASTContext &C) override;
private:
    DCCVisitor V;
};

#endif // DCCCONSUMER_H
