#ifndef LLVM_NODE_DI_VARIABLE_H
#define LLVM_NODE_DI_VARIABLE_H

#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "../util/from-value-mixin.h"

class DIVariableWrapper: public Nan::ObjectWrap, public FromValueMixin<DIVariableWrapper> {
    public:
        static NAN_MODULE_INIT(Init);
        llvm::DIVariable *getDIVariable();
        static v8::Local<v8::Object> of(llvm::DIVariable *);
        static bool isInstance(v8::Local<v8::Value>);

        DIVariableWrapper(llvm::DIVariable *);
    
    private:
        llvm::DIVariable *diVariable;

        static NAN_METHOD(New);
        static NAN_METHOD(getLine);
        static NAN_METHOD(getScope);
        static NAN_METHOD(getName);
        static NAN_METHOD(getFile);
        static NAN_METHOD(getType);
        static NAN_METHOD(getFilename);
        static NAN_METHOD(getDirectory);
        static NAN_METHOD(getSizeInBits);
    protected:
        static Nan::Persistent<v8::FunctionTemplate> &diVariableTemplate();
};

#endif //LLVM_NODE_DI_VARIABLE_H