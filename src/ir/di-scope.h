#ifndef LLVM_NODE_DI_SCOPE_H
#define LLVM_NODE_DI_SCOPE_H
#include <nan.h>

#include <llvm/IR/DebugInfoMetadata.h>
#include "../util/from-value-mixin.h"

class DIScopeWrapper: public Nan::ObjectWrap, public FromValueMixin<DIScopeWrapper> {
    public:
        static NAN_MODULE_INIT(Init);
        static v8::Local<v8::Object> of(llvm::DIScope *);
        llvm::DIScope *getDIScope();
        static bool isInstance(v8::Local<v8::Value>);

        DIScopeWrapper(llvm::DIScope *);
    
    private:
        llvm::DIScope *diScope;

        static NAN_METHOD(New);
        static NAN_METHOD(getFile);
        static NAN_METHOD(getFilename);
        static NAN_METHOD(getDirectory);
        static NAN_METHOD(getName);
    protected:
        static Nan::Persistent<v8::FunctionTemplate> &diScopeTemplate();
};

#endif //LLVM_NODE_DI_SCOPE_H