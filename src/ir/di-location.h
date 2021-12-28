#ifndef LLVM_NODE_DI_LOCATION_H
#define LLVM_NODE_DI_LOCATION_H

#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "../util/from-value-mixin.h"

class DILocationWrapper: public Nan::ObjectWrap, public FromValueMixin<DILocationWrapper> {
    public:
        static NAN_MODULE_INIT(Init);
        llvm::DILocation *getDILocation();
        static v8::Local<v8::Object> of(llvm::DILocation *);
        static bool isInstance(v8::Local<v8::Value>);

        DILocationWrapper(llvm::DILocation *);
    
    private:
        llvm::DILocation *diLocation;

        static NAN_METHOD(New);
        static NAN_METHOD(get);
        static Nan::Persistent<v8::FunctionTemplate> &diLocationTemplate();
};

#endif //LLVM_NODE_DI_LOCATION_H