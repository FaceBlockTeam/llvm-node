#ifndef LLVM_NODE_DI_NODE_ARRAY_H
#define LLVM_NODE_DI_NODE_ARRAY_H
#include <nan.h>

#include <llvm/IR/DebugInfoMetadata.h>
#include "../util/from-value-mixin.h"

class DINodeArrayWrapper: public Nan::ObjectWrap, public FromValueMixin<DINodeArrayWrapper> {
    public:
        static NAN_MODULE_INIT(Init);
        static v8::Local<v8::Object> of(llvm::DINodeArray);
        llvm::DINodeArray getDINodeArray();
        static bool isInstance(v8::Local<v8::Value>);
        DINodeArrayWrapper(llvm::DINodeArray);
    
    private:
        llvm::DINodeArray diNodeArray;

        static NAN_METHOD(New);

        static Nan::Persistent<v8::FunctionTemplate> &diNodeArrayTemplate();
};

#endif //LLVM_NODE_DI_NODE_ARRAY_H