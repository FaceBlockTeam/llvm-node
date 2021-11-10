#ifndef LLVM_NODE_INVOKEINST_H
#define LLVM_NODE_INVOKEINST_H

#include <nan.h>
#include <llvm/IR/Instructions.h>
#include "value.h"

class InvokeInstWrapper: public ValueWrapper, public FromValueMixin<InvokeInstWrapper> {
public:
    using FromValueMixin<InvokeInstWrapper>::FromValue;

    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::InvokeInst* invokeInst);
    llvm::InvokeInst* getInvokeInst();

private:
    explicit InvokeInstWrapper(llvm::InvokeInst* invokeInst): ValueWrapper { invokeInst } {
    }

    static NAN_METHOD(New);
    static NAN_GETTER(getNormalDest);
    static NAN_SETTER(setNormalDest);
    static NAN_GETTER(getUnwindDest);
    static NAN_SETTER(setUnwindDest);
    static NAN_GETTER(getLandingPadInst);
    static NAN_GETTER(getNumSuccessors);
    static NAN_METHOD(getSuccessor);
    static NAN_METHOD(setSuccessor);
    static Nan::Persistent<v8::FunctionTemplate>& invokeInstTemplate();
};

#endif //LLVM_NODE_INVOKEINST_H