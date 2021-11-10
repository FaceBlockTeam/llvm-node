#include <nan.h>
#include "corosave-inst.h"
#include "corosuspend-inst.h"
#include <Transforms/Coroutines/CoroInstr.h>

NAN_MODULE_INIT(CoroSuspendInstWrapper::Init) {
    auto coroSuspendInst = Nan::GetFunction(Nan::New(coroSuspendInstTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("CoroSuspendInst").ToLocalChecked(), coroSuspendInst);
}

NAN_METHOD(CoroSuspendInstWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *coroSuspendInst = static_cast<llvm::CoroSuspendInst*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new CoroSuspendInstWrapper { coroSuspendInst };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(CoroSuspendInstWrapper::getCoroSave) {
    auto* coroSuspendInst = CoroSuspendInstWrapper::FromValue(info.Holder())->getCoroSuspendInst();
    auto* coroSaveInst = coroSuspendInst->getCoroSave();
    info.GetReturnValue().Set(CoroSaveInstWrapper::of(coroSaveInst));
}

NAN_METHOD(CoroSuspendInstWrapper::isFinal) {
    auto* coroSuspendInst = CoroSuspendInstWrapper::FromValue(info.Holder())->getCoroSuspendInst();
    bool isFinal = coroSuspendInst->isFinal();
    info.GetReturnValue().Set(isFinal);
}

v8::Local<v8::Object> CoroSuspendInstWrapper::of(llvm::CoroSuspendInst* coroSuspendInst) {
    auto constructorFunction = Nan::GetFunction(Nan::New(coroSuspendInstTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(coroSuspendInst)};

    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapableHandleScope {};
    return escapableHandleScope.Escape(instance);
}

llvm::CoroSuspendInst* CoroSuspendInstWrapper::getCoroSuspendInst() {
    return static_cast<llvm::CoroSuspendInst*>(getValue());
}

Nan::Persistent<v8::FunctionTemplate>& CoroSuspendInstWrapper::coroSuspendInstTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> tmpl {};

    if (tmpl.IsEmpty()) {
        auto coroSuspendInstWrapperTemplate = Nan::New<v8::FunctionTemplate>(CoroSuspendInstWrapper::New);
        coroSuspendInstWrapperTemplate->SetClassName(Nan::New("CoroSuspendInst").ToLocalChecked());
        coroSuspendInstWrapperTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        coroSuspendInstWrapperTemplate->Inherit(Nan::New(anyCoroSuspendInstTemplate()));

        Nan::SetPrototypeMethod(coroSuspendInstWrapperTemplate, "getCoroSave", getCoroSave);
        Nan::SetPrototypeMethod(coroSuspendInstWrapperTemplate, "isFinal", isFinal);

        tmpl.Reset(anyCoroIdInstWrapperTemplate);
    }
}