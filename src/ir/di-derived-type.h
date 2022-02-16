#ifndef LLVM_NODE_DI_DERIVED_TYPE_H
#define LLVM_NODE_DI_DERIVED_TYPE_H
#include <nan.h>

#include <llvm/IR/DebugInfoMetadata.h>
#include "../util/from-value-mixin.h"
#include "di-type.h"

class DIDerivedTypeWrapper: public DITypeWrapper, public FromValueMixin<DIDerivedTypeWrapper> {
    public:
        static NAN_MODULE_INIT(Init);
        static v8::Local<v8::Object> of(llvm::DIDerivedType *);
        using FromValueMixin<DIDerivedTypeWrapper>::FromValue;
        DIDerivedTypeWrapper(llvm::DIDerivedType *);
    
    private:
        llvm::DIDerivedType *diDerivedType;

        static NAN_METHOD(New);

        static Nan::Persistent<v8::FunctionTemplate> &diDerivedTypeTemplate();
};

#endif //LLVM_NODE_DI_DERIVED_TYPE_H