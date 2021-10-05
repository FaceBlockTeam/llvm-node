#ifndef LLVM_NODE_INTRINSICINST_H
#define LLVM_NODE_INTRINSICINST_H

#include <nan.h>
#include <llvm/IR/IntrinsicInst.h>
#include "value.h"

class IntrinsicInstWrapper: public ValueWrapper, public FromValueMixin<IntrinsicInstWrapper> {
public:
    using FromValueMixin<IntrinsicInstWrapper>::FromValue;

    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::IntrinsicInst* intrinsicInst);
    llvm::IntrinsicInst* getIntrinsicInst();
protected:
    static Nan::Persistent<v8::FunctionTemplate>& intrinsicInstTemplate();

private:
    explicit IntrinsicInstWrapper(llvm::IntrinsicInst* intrinsicInst): ValueWrapper { intrinsicInst } {
    }

    static NAN_METHOD(New);
    static NAN_METHOD(getIntrinsicID);
};

#endif //LLVM_NODE_INTRINSICINST_H