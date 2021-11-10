#ifndef LLVM_NODE_COROENDINST_H
#define LLVM_NODE_COROENDINST_H

#include <nan.h>
#include "Transforms/Coroutines/CoroInstr.h"
#include "anycoroend-inst.h"

class CoroEndInstWrapper: public AnyCoroEndInstWrapper, public FromValueMixin<CoroEndInstWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::CoroEndInst* coroEndInst);
    using FromValueMixin<CoroEndInstWrapper>::FromValue;
    llvm::CoroEndInst* getCoroEndInst();

private:
    explicit CoroEndInstWrapper(llvm::CoroEndInst* coroEndInst) : AnyCoroEndInstWrapper { coroEndInst }
    {}

    static Nan::Persistent<v8::FunctionTemplate>& coroEndInstTemplate();

    static NAN_METHOD(New);
};

#endif //LLVM_NODE_COROENDINST_H