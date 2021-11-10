#ifndef LLVM_NODE_SWITCHINST_H
#define LLVM_NODE_SWITCHINST_H

#include <nan.h>
#include <llvm/IR/Instructions.h>
#include "value.h"

class SwitchInstWrapper: public ValueWrapper, public FromValueMixin<SwitchInstWrapper> {
public:
    using FromValueMixin<SwitchInstWrapper>::FromValue;

    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::SwitchInst* switchInst);
    llvm::SwitchInst* getSwitchInst();

private:
    explicit SwitchInstWrapper(llvm::SwitchInst* switchInst): ValueWrapper { switchInst } {
    }

    static NAN_METHOD(New);
    static NAN_METHOD(create);
    static NAN_GETTER(getCondition);
    static NAN_SETTER(setCondition);
    static NAN_GETTER(getDefaultDest);
    static NAN_SETTER(setDefaultDest);
    static NAN_GETTER(getNumCases);
    static NAN_METHOD(addCase);
    static NAN_GETTER(getNumSuccessors);
    static NAN_METHOD(getSuccessor);
    static NAN_METHOD(setSuccessor);
    static Nan::Persistent<v8::FunctionTemplate>& switchInstTemplate();
};

#endif //LLVM_NODE_SWITCHINST_H