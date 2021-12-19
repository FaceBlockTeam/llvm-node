#ifndef LLVM_NODE_DI_MODULE_H
#define LLVM_NODE_DI_MODULE_H
#include <nan.h>

#include <llvm/IR/DebugInfoMetadata.h>
#include "../util/from-value-mixin.h"

class DIModuleWrapper: public Nan::ObjectWrap, public FromValueMixin<DIModuleWrapper> {
    public:
        static NAN_MODULE_INIT(Init);
        static v8::Local<v8::Object> of(llvm::DIModule *);

        DIModuleWrapper(llvm::DIModule *);
    
    private:
        llvm::DIModule *diModule;

        static NAN_METHOD(New);
        static Nan::Persistent<v8::FunctionTemplate> &diModuleTemplate();
};

#endif //LLVM_NODE_DI_MODULE_H