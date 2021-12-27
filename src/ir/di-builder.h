#ifndef LLVM_NODE_DI_BUILDER_H
#define LLVM_NODE_DI_BUILDER_H

#include <nan.h>
#include <llvm/IR/DIBuilder.h>
#include "../util/from-value-mixin.h"

class DIBuilderWrapper: public Nan::ObjectWrap, public FromValueMixin<DIBuilderWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    llvm::DIBuilder &getDIBuilder();

private:
    llvm::DIBuilder diBuilder;

    explicit DIBuilderWrapper(llvm::Module &, bool, llvm::DICompileUnit *);

    static NAN_METHOD(New);
    static NAN_METHOD(finalize);
    static NAN_METHOD(finalizeSubprogram);
    static NAN_METHOD(createCompileUnit);
    static NAN_METHOD(createFile);
    static NAN_METHOD(createInheritance);
    static NAN_METHOD(createExpression);
    static NAN_METHOD(createFunction);
    static NAN_METHOD(createModule);

    static Nan::Persistent<v8::FunctionTemplate> &diBuilderTemplate();
};

#endif //LLVM_NODE_DI_BUILDER_H
