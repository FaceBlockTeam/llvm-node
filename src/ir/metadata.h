#ifndef LLVM_NODE_METADATA_H
#define LLVM_NODE_METADATA_H
#include <nan.h>
#include "llvm/IR/Metadata.h"
#include "../util/from-value-mixin.h"

class MetadataWrapper: public Nan::ObjectWrap, public FromValueMixin<MetadataWrapper> {
    public:
        static NAN_MODULE_INIT(Init);
        llvm::Metadata *getMetadata();
        static bool isInstance(v8::Local<v8::Value>);

        MetadataWrapper(llvm::Metadata *);
    
    private:
        llvm::Metadata *metadata;

        static NAN_METHOD(New);

        static Nan::Persistent<v8::FunctionTemplate> &metadataTemplate();
};

#endif //LLVM_NODE_METADATA_H