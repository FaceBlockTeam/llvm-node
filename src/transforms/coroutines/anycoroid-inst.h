#ifndef LLVM_NODE_ANYCOROIDINST_H
#define LLVM_NODE_ANYCOROIDINST_H

#include <nan.h>
#include "Transforms/Coroutines/CoroInstr.h"
#include "../../ir/intrinsic-inst.h"

class AnyCoroIdInstWrapper: public IntrinsicInstWrapper, public FromValueMixin<AnyCoroIdInstWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::AnyCoroIdInst* anyCoroIdInst);
    using FromValueMixin<AnyCoroIdInstWrapper>::FromValue;
    llvm::AnyCoroIdInst* getAnyCoroIdInst();

private:
    explicit AnyCoroIdInstWrapper(llvm::AnyCoroIdInst* anyCoroIdInst) : IntrinsicInstWrapper { anyCoroIdInst }
    {}

    static Nan::Persistent<v8::FunctionTemplate>& anyCoroIdInstTemplate();

    static NAN_METHOD(New);
    static NAN_METHOD(getCoroAlloc);
    static NAN_METHOD(getCoroBegin);
};

#endif //LLVM_NODE_COROBEGININST_H
