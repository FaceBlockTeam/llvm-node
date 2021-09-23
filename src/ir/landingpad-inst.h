#ifndef LLVM_NODE_LANDINGPADINST_H
#define LLVM_NODE_LANDINGPADINST_H

#include <nan.h>
#include <llvm/IR/Instructions.h>
#include "value.h"

class LandingPadInstWrapper: public ValueWrapper, public FromValueMixin<LandingPadInstWrapper> {
public:
    using FromValueMixin<LandingPadInstWrapper>::FromValue;

    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::LandingPadInst* landingPadInst);
    llvm::LandingPadInst* getLandingPadInst();

private:
    explicit LandingPadInstWrapper(llvm::LandingPadInst* landingPadInst): ValueWrapper { landingPadInst } {
    }

    static NAN_METHOD(New);
    static NAN_GETTER(getNumClauses);
    static NAN_METHOD(addClause);
    static NAN_METHOD(getClause);
    static NAN_SETTER(setCleanup);
    static NAN_GETTER(isCleanup);
    static NAN_METHOD(isCatch);
    static NAN_METHOD(isFilter);
    static Nan::Persistent<v8::FunctionTemplate>& landingPadInstTemplate();
};

#endif //LLVM_NODE_LANDINGPADINST_H