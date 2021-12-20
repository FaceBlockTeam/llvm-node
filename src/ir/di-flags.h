#ifndef LLVM_NODE_DI_FLAGS_H
#define LLVM_NODE_DI_FLAGS_H

#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "../util/from-value-mixin.h"

class DIFlagsWrapper: public Nan::ObjectWrap, public FromValueMixin<DIFlagsWrapper> {
    public:
        static NAN_MODULE_INIT(Init);
        llvm::DINode::DIFlags getDIFlags();
        static v8::Local<v8::Object> of(llvm::DINode::DIFlags);
        static bool isInstance(v8::Local<v8::Value>);

        DIFlagsWrapper(llvm::DINode::DIFlags);
    
    private:
        llvm::DINode::DIFlags diFlags;

        static NAN_METHOD(New);

        static Nan::Persistent<v8::FunctionTemplate> &diFlagsTemplate();
};

#endif //LLVM_NODE_DI_FLAGS_H