#ifndef LLVM_NODE_DI_COMPILE_UNIT_H
#define LLVM_NODE_DI_COMPILE_UNIT_H

#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "../util/from-value-mixin.h"

class DICompileUnitWrapper: public Nan::ObjectWrap, public FromValueMixin<DICompileUnitWrapper> {
    public:
        static NAN_MODULE_INIT(Init);
        static v8::Local<v8::Object> of(llvm::DICompileUnit *);
        llvm::DICompileUnit *getDICompileUnit();
        static bool isInstance(v8::Local<v8::Value>);

        DICompileUnitWrapper(llvm::DICompileUnit *);
    
    private:
        llvm::DICompileUnit *diCompileUnit;

        static NAN_METHOD(New);

        static Nan::Persistent<v8::FunctionTemplate> &diCompileUnitTemplate();
};

#endif //LLVM_NODE_DI_COMPILE_UNIT_H
