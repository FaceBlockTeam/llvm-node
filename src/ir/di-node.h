#ifndef LLVM_NODE_DI_NODE_H
#define LLVM_NODE_DI_NODE_H

#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "../util/from-value-mixin.h"

class DINodeWrapper: public Nan::ObjectWrap, public FromValueMixin<DINodeWrapper> {
    public:
        static NAN_MODULE_INIT(Init);
        llvm::DINode *getDINode();
        static v8::Local<v8::Object> of(llvm::DINode *);
        static bool isInstance(v8::Local<v8::Value>);

        DINodeWrapper(llvm::DINode *);
    
    private:
        llvm::DINode *diNode;

        static NAN_METHOD(New);

        static Nan::Persistent<v8::FunctionTemplate> &diNodeTemplate();
};

#endif //LLVM_NODE_DI_NODE_H