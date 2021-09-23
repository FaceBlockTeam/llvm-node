#ifndef LLVM_NODE_CATCHSWITCHINST_H
#define LLVM_NODE_CATCHSWITCHINST_H

#include <nan.h>
#include <llvm/IR/Instructions.h>
#include "value.h"

class CatchSwitchInstWrapper: public ValueWrapper, public FromValueMixin<CatchSwitchInstWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::BasicBlock* basicBlock);
    static bool isInstance(v8::Local<v8::Value> value);
    using FromValueMixin<CatchSwitchInstWrapper>::FromValue;
    llvm::CatchSwitchInst* getCatchSwitchInst();

private:
    explicit CatchSwitchInstWrapper(llvm::CatchSwitchInst* catchSwitchInst)
            : ValueWrapper { catchSwitchInst }
    {}

    static NAN_METHOD(New);
    static NAN_METHOD(addHandler);
    static NAN_METHOD(create);
    static NAN_GETTER(getParentPad);
    static NAN_SETTER(setParentPad);
    static NAN_GETTER(hasUnwindDest);
    static NAN_GETTER(unwindsToCaller);
    static NAN_GETTER(getUnwindDest);
    static NAN_SETTER(setUnwindDest);
    static NAN_GETTER(getNumHandlers);

    static Nan::Persistent<v8::FunctionTemplate>& catchSwitchInstTemplate();
};


#endif //LLVM_NODE_CATCHSWITCHINST_H
