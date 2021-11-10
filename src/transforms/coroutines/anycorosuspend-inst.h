#ifndef LLVM_NODE_ANYCOROSUSPENDINST_H
#define LLVM_NODE_ANYCOROSUSPENDINST_H

#include <nan.h>
#include "Transforms/Coroutines/CoroInstr.h"
#include "../../ir/intrinsic-inst.h"

class AnyCoroSuspendInstWrapper: public IntrinsicInstWrapper, public FromValueMixin<AnyCoroSuspendInstWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::AnyCoroSuspendInst* anyCoroSuspendInst);
    using FromValueMixin<AnyCoroSuspendInstWrapper>::FromValue;
    llvm::AnyCoroSuspendInst* getAnyCoroSuspendInst();

private:
    explicit AnyCoroSuspendInstWrapper(llvm::AnyCoroSuspendInst* anyCoroSuspendInst) : IntrinsicInstWrapper { anyCoroSuspendInst }
    {}

    static Nan::Persistent<v8::FunctionTemplate>& anyCoroSuspendInstTemplate();

    static NAN_METHOD(New);
    static NAN_METHOD(getCoroSave);
};

#endif //LLVM_NODE_ANYCOROSUSPENDINST_H