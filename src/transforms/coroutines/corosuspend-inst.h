#ifndef LLVM_NODE_COROSUSPENDINST_H
#define LLVM_NODE_COROSUSPENDINST_H

#include <nan.h>
#include "Transforms/Coroutines/CoroInstr.h"
#include "anycorosuspend-inst.h"

class CoroSuspendInstWrapper: public AnyCoroSuspendInstWrapper, public FromValueMixin<CoroSuspendInstWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::CoroSuspendInst* coroSuspendInst);
    using FromValueMixin<CoroSuspendInstWrapper>::FromValue;
    llvm::CoroSuspendInst* getCoroSuspendInst();

private:
    explicit CoroSuspendInst(llvm::CoroSuspendInst* coroSuspendInst) : IntrinsicInstWrapper { coroSuspendInst }
    {}

    static Nan::Persistent<v8::FunctionTemplate>& coroSuspendInstTemplate();

    static NAN_METHOD(New);
    static NAN_METHOD(getCoroSave);
    static NAN_METHOD(isFinal);
};

#endif //LLVM_NODE_COROSUSPENDINST_H
