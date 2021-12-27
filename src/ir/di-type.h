#ifndef LLVM_NODE_DI_TYPE_H
#define LLVM_NODE_DI_TYPE_H
#include <nan.h>

#include <llvm/IR/DebugInfoMetadata.h>
#include "../util/from-value-mixin.h"

class DITypeWrapper: public Nan::ObjectWrap, public FromValueMixin<DITypeWrapper> {
    public:
        static NAN_MODULE_INIT(Init);
        static v8::Local<v8::Object> of(llvm::DIType *);
        llvm::DIType *getDIType();
        static bool isInstance(v8::Local<v8::Value>);

        DITypeWrapper(llvm::DIType *);
    
    private:
        llvm::DIType *diType;

        static NAN_METHOD(New);
        static NAN_METHOD(getLine);
        static NAN_METHOD(getScope);
        static NAN_METHOD(getName);

        static Nan::Persistent<v8::FunctionTemplate> &diTypeTemplate();
};

#endif //LLVM_NODE_DI_TYPE_H