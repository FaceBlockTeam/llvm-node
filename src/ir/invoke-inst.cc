#include <llvm/IR/Instructions.h>
#include <nan.h>
#include "basic-block.h"
#include "invoke-inst.h"
#include "landingpad-inst.h"

NAN_MODULE_INIT(InvokeInstWrapper::Init) {
    auto invokeInst = Nan::GetFunction(Nan::New(invokeInstTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("InvokeInst").ToLocalChecked(), invokeInst);
}

NAN_METHOD(InvokeInstWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("The constructor of InvokeInst needs to be called with new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("The constructor of InvokeInst needs to be called with InvokeInst: External");
    }

    auto* value = static_cast<llvm::InvokeInst*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new InvokeInstWrapper { value };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_GETTER(InvokeInstWrapper::getNormalDest) {
    auto* invokeInst = InvokeInstWrapper::FromValue(info.Holder())->getInvokeInst();
    auto* normalDest = invokeInst->getNormalDest();
    info.GetReturnValue().Set(BasicBlockWrapper::of(normalDest));
}

NAN_SETTER(InvokeInstWrapper::setNormalDest) {
    if (!BasicBlockWrapper::isInstance(value)) {
        return Nan::ThrowTypeError("setUnwindDest needs to be called with unwindDest: BasicBlock");
    }

    auto* normalDest = BasicBlockWrapper::FromValue(value)->getBasicBlock();
    auto* invokeInst = InvokeInstWrapper::FromValue(info.Holder())->getInvokeInst();
    invokeInst->setNormalDest(normalDest);
}

NAN_GETTER(InvokeInstWrapper::getUnwindDest) {
    auto* invokeInst = InvokeInstWrapper::FromValue(info.Holder())->getInvokeInst();
    auto* unwindDest = invokeInst->getUnwindDest();
    info.GetReturnValue().Set(BasicBlockWrapper::of(unwindDest));
}

NAN_SETTER(InvokeInstWrapper::setUnwindDest) {
    if (!BasicBlockWrapper::isInstance(value)) {
        return Nan::ThrowTypeError("setUnwindDest needs to be called with unwindDest: BasicBlock");
    }

    auto* unwindDest = BasicBlockWrapper::FromValue(value)->getBasicBlock();
    auto* invokeInst = InvokeInstWrapper::FromValue(info.Holder())->getInvokeInst();
    invokeInst->setUnwindDest(unwindDest);
}

NAN_GETTER(InvokeInstWrapper::getLandingPadInst) {
    auto* invokeInst = InvokeInstWrapper::FromValue(info.Holder())->getInvokeInst();
    auto* landingPadInst = invokeInst->getLandingPadInst();
    info.GetReturnValue().Set(LandingPadInstWrapper::of(landingPadInst));
}

NAN_GETTER(InvokeInstWrapper::getNumSuccessors) {
    auto* invokeInst = InvokeInstWrapper::FromValue(info.Holder())->getInvokeInst();
    uint32_t numSuccessors = invokeInst->getNumSuccessors();
    info.GetReturnValue().Set(numSuccessors);
}

NAN_METHOD(InvokeInstWrapper::getSuccessor) {
    if (info.Length() != 1) {
        return Nan::ThrowTypeError("getSuccessor needs to be called with idx: number");
    }

    uint32_t idx = Nan::To<uint32_t>(info[0]).FromJust();

    auto* invokeInst = InvokeInstWrapper::FromValue(info.Holder())->getInvokeInst();
    auto* successor = invokeInst->getSuccessor(idx);
    info.GetReturnValue().Set(BasicBlockWrapper::of(successor));
}

NAN_METHOD(InvokeInstWrapper::setSuccessor) {
    if (info.Length() != 2 || !info[0]->IsUint32() || !BasicBlockWrapper::isInstance(info[1])) {
        return Nan::ThrowTypeError("setSuccessor needs to be called with idx: number, newSuccessor: BasicBlock");
    }

    uint32_t idx = Nan::To<uint32_t>(info[0]).FromJust();
    auto* successor = BasicBlockWrapper::FromValue(info[1])->getBasicBlock();

    auto* invokeInst = InvokeInstWrapper::FromValue(info.Holder())->getInvokeInst();
    invokeInst->setSuccessor(idx, successor);
}

v8::Local<v8::Object> InvokeInstWrapper::of(llvm::InvokeInst *invokeInst) {
    auto constructorFunction = Nan::GetFunction(Nan::New(invokeInstTemplate())).ToLocalChecked();
    v8::Local<v8::Value> argv[1] = { Nan::New<v8::External>(invokeInst) };
    auto instance = Nan::NewInstance(constructorFunction, 1, argv).ToLocalChecked();

    Nan::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(instance);
}

Nan::Persistent<v8::FunctionTemplate>& InvokeInstWrapper::invokeInstTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(InvokeInstWrapper::New);
        localTemplate->SetClassName(Nan::New("InvokeInst").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        localTemplate->Inherit(Nan::New(valueTemplate()));

        Nan::SetMethod(localTemplate, "getSuccessor", InvokeInstWrapper::getSuccessor);
        Nan::SetMethod(localTemplate, "setSuccessor", InvokeInstWrapper::setSuccessor);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("normalDest").ToLocalChecked(), InvokeInstWrapper::getNormalDest, InvokeInstWrapper::setNormalDest);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("unwindDest").ToLocalChecked(), InvokeInstWrapper::getUnwindDest, InvokeInstWrapper::setUnwindDest);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("landingPadInst").ToLocalChecked(), InvokeInstWrapper::getLandingPadInst);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("numSuccessors").ToLocalChecked(), InvokeInstWrapper::getNumSuccessors);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}

llvm::InvokeInst* InvokeInstWrapper::getInvokeInst() {
    return static_cast<llvm::InvokeInst*>(getValue());
}