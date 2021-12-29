#ifndef LLVM_NODE_DI_TYPE_REF_ARRAY_H
#define LLVM_NODE_DI_TYPE_REF_ARRAY_H
#include <nan.h>

#include <llvm/IR/DebugInfoMetadata.h>
#include "../util/from-value-mixin.h"

class DITypeRefArrayWrapper: public Nan::ObjectWrap, public FromValueMixin<DITypeRefArrayWrapper> {
    public:
        static NAN_MODULE_INIT(Init);
        static v8::Local<v8::Object> of(llvm::DITypeRefArray);
        llvm::DITypeRefArray getDITypeRefArray();
        static bool isInstance(v8::Local<v8::Value>);
        DITypeRefArrayWrapper(llvm::DITypeRefArray);
    
    private:
        llvm::DITypeRefArray diTypeRefArray;

        static NAN_METHOD(New);

        static Nan::Persistent<v8::FunctionTemplate> &diTypeRefArrayTemplate();
};

#endif //LLVM_NODE_DI_TYPE_REF_ARRAY_H