#ifndef LLVM_NODE_ANYCOROENDINST_H
#define LLVM_NODE_ANYCOROENDINST_H

#include <nan.h>
#include "Transforms/Coroutines/CoroInstr.h"
#include "intrinsic-inst.h"

class AnyCoroEndInstWrapper: public IntrinsicInstWrapper, public FromValueMixin<AnyCoroEndInstWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::AnyCoroEndInst* anyCoroEndInst);
    using FromValueMixin<AnyCoroEndInstWrapper>::FromValue;
    llvm::AnyCoroEndInst* getAnyCoroEndInst();

private:
    explicit CoroEndInstWrapper(llvm::AnyCoroEndInst* AnyCoroEndInst) : AnyCoroEndInstWrapper { anyCoroEndInst }
    {}

    static Nan::Persistent<v8::FunctionTemplate>& anyCoroEndInstTemplate();

    static NAN_METHOD(New);
};

#endif //LLVM_NODE_ANYCOROENDINST_H