#include "DCCConsumer.h"

DCCConsumer::DCCConsumer(clang::ASTContext &C) : V(C) {}

void DCCConsumer::HandleTranslationUnit(clang::ASTContext &C) {
    V.TraverseDecl(C.getTranslationUnitDecl());
}
