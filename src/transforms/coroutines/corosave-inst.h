#ifndef LLVM_NODE_COROSAVEINST_H
#define LLVM_NODE_COROSAVEINST_H

#include <nan.h>
#include "Transforms/Coroutines/CoroInstr.h"
#include "../../ir/intrinsic-inst.h"

class CoroSaveInstWrapper: public IntrinsicInstWrapper, public FromValueMixin<CoroSaveInstWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::CoroSaveInst* coroSaveInst);
    using FromValueMixin<CoroSaveInstWrapper>::FromValue;
    llvm::CoroSaveInst* getCoroSaveInst();

private:
    explicit CoroSaveInstWrapper(llvm::CoroSaveInst* coroSaveInst) : IntrinsicInstWrapper { coroSaveInst }
    {}

    static Nan::Persistent<v8::FunctionTemplate>& coroSaveInstTemplate();

    static NAN_METHOD(New);
};

#endif //LLVM_NODE_COROSAVEINST_H
