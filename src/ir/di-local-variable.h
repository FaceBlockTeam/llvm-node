#ifndef LLVM_NODE_DI_LOCAL_VARIABLE_H
#define LLVM_NODE_DI_LOCAL_VARIABLE_H

#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "../util/from-value-mixin.h"
#include "di-variable.h"

class DILocalVariableWrapper: public DIVariableWrapper, public FromValueMixin<DILocalVariableWrapper> {
    public:
        static NAN_MODULE_INIT(Init);
        llvm::DILocalVariable *getDILocalVariable();
        static v8::Local<v8::Object> of(llvm::DILocalVariable *);
        static bool isInstance(v8::Local<v8::Value>);
        using FromValueMixin<DILocalVariableWrapper>::FromValue;
        DILocalVariableWrapper(llvm::DILocalVariable *);
    
    private:
        llvm::DILocalVariable *diLocalVariable;

        static NAN_METHOD(New);
        static NAN_METHOD(getScope);
        static NAN_METHOD(isParameter);
        static Nan::Persistent<v8::FunctionTemplate> &diLocalVariableTemplate();
};

#endif //LLVM_NODE_DI_LOCAL_VARIABLE_H