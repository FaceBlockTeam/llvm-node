#ifndef LLVM_NODE_DI_BASIC_TYPE_H
#define LLVM_NODE_DI_BASIC_TYPE_H

#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "../util/from-value-mixin.h"

class DIBasicTypeWrapper: public Nan::ObjectWrap, public FromValueMixin<DIBasicTypeWrapper> {
    public:
        static NAN_MODULE_INIT(Init);
        static v8::Local<v8::Object> of(llvm::DIBasicType *);
        llvm::DIBasicType *getDIBasicType();
        static bool isInstance(v8::Local<v8::Value>);
        DIBasicTypeWrapper(llvm::DIBasicType *);
    
    private:
        llvm::DIBasicType *diBasicType;

        static NAN_METHOD(New);

        static Nan::Persistent<v8::FunctionTemplate> &diBasicTypeTemplate();
};

#endif //LLVM_NODE_DI_BASIC_TYPE_H