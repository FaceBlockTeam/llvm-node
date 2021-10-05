#ifndef LLVM_NODE_COROALLOCINST_H
#define LLVM_NODE_COROALLOCINST_H

#include <nan.h>
#include "Transforms/Coroutines/CoroInstr.h"
#include "../../ir/intrinsic-inst.h"

class CoroAllocInstWrapper: public IntrinsicInstWrapper, public FromValueMixin<CoroAllocInstWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::CoroAllocInst* coroAllocInst);
    using FromValueMixin<CoroAllocInstWrapper>::FromValue;
    llvm::CoroAllocInst* getCoroAllocInst();

private:
    explicit CoroAllocInstWrapper(llvm::CoroAllocInst* coroAllocInst) : IntrinsicInstWrapper { coroAllocInst }
    {}

    static Nan::Persistent<v8::FunctionTemplate>& coroAllocInstTemplate();

    static NAN_METHOD(New);
};

#endif //LLVM_NODE_COROALLOCINST_H