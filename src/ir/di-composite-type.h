#ifndef LLVM_NODE_DI_COMPOSITE_TYPE_H
#define LLVM_NODE_DI_COMPOSITE_TYPE_H

#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "../util/from-value-mixin.h"
#include "di-type.h"

class DICompositeTypeWrapper: public DITypeWrapper, public FromValueMixin<DICompositeTypeWrapper> {
    public:
        static NAN_MODULE_INIT(Init);
        static v8::Local<v8::Object> of(llvm::DICompositeType *);
        llvm::DICompositeType *getDICompositeType();
        static bool isInstance(v8::Local<v8::Value>);
        using FromValueMixin<DICompositeTypeWrapper>::FromValue;
        DICompositeTypeWrapper(llvm::DICompositeType *);
    
    private:
        llvm::DICompositeType *diCompositeType;

        static NAN_METHOD(New);

        static Nan::Persistent<v8::FunctionTemplate> &diCompositeTypeTemplate();
};

#endif //LLVM_NODE_DI_COMPOSITE_TYPE_H