#ifndef LLVM_NODE_DI_SUBROUTINE_TYPE_H
#define LLVM_NODE_DI_SUBROUTINE_TYPE_H
#include <nan.h>

#include <llvm/IR/DebugInfoMetadata.h>
#include "../util/from-value-mixin.h"
#include "di-type.h"

class DISubroutineTypeWrapper: public DITypeWrapper, public FromValueMixin<DISubroutineTypeWrapper> {
    public:
        static NAN_MODULE_INIT(Init);
        static v8::Local<v8::Object> of(llvm::DISubroutineType *);
        llvm::DISubroutineType *getDISubroutineType();
        static bool isInstance(v8::Local<v8::Value>);
        using FromValueMixin<DISubroutineTypeWrapper>::FromValue;
        DISubroutineTypeWrapper(llvm::DISubroutineType *);
    
    private:
        llvm::DISubroutineType *diSubroutineType;

        static NAN_METHOD(New);

        static Nan::Persistent<v8::FunctionTemplate> &diSubroutineTypeTemplate();
};

#endif //LLVM_NODE_DI_SUBROUTINE_TYPE_H