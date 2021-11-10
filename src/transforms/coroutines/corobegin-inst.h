#ifndef LLVM_NODE_COROBEGININST_H
#define LLVM_NODE_COROBEGININST_H

#include <nan.h>
#include "Transforms/Coroutines/CoroInstr.h"
#include "../../ir/intrinsic-inst.h"

class CoroBeginInstWrapper: public IntrinsicInstWrapper, public FromValueMixin<CoroBeginInstWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::CoroBeginInst* coroBeginInst);
    using FromValueMixin<CoroBeginInstWrapper>::FromValue;
    llvm::CoroBeginInst* getCoroBeginInst();

private:
    explicit CoroBeginInstWrapper(llvm::CoroBeginInst* coroBeginInst) : IntrinsicInstWrapper { coroBeginInst }
    {}

    static Nan::Persistent<v8::FunctionTemplate>& coroBeginInstTemplate();

    static NAN_METHOD(New);
};

#endif //LLVM_NODE_COROBEGININST_H
