#include <nan.h>
#include "Transforms/Coroutines/CoroInstr.h"
#include "anycorosuspend-inst.h"
#include "corosave-inst.h"

NAN_MODULE_INIT(AnyCoroSuspendInstWrapper::Init) {
    auto anyCoroSuspendInst = Nan::GetFunction(Nan::New(anyCoroSuspendInstTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("AnyCoroSuspendInst").ToLocalChecked(), anyCoroSuspendInst);
}

NAN_METHOD(AnyCoroSuspendInstWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *anyCoroSuspendInst = static_cast<llvm::AnyCoroSuspendInst*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new AnyCoroSuspendInstWrapper { anyCoroSuspendInst };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(AnyCoroSuspendInstWrapper::getCoroSave) {
    auto* anyCoroSuspendInst= AnyCoroSuspendInstWrapper::FromValue(info.Holder())->getAnyCoroSuspendInst();
    auto* coroSaveInst = anyCoroSuspendInst->getCoroSave();
    info.GetReturnValue().Set(CoroSaveInstWrapper::of(coroSaveInst));
}

v8::Local<v8::Object> AnyCoroSuspendInstWrapper::of(llvm::AnyCoroSuspendInst* anyCoroSuspendInst) {
    auto constructorFunction = Nan::GetFunction(Nan::New(anyCoroSuspendInstTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(anyCoroSuspendInst)};

    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapableHandleScope {};
    return escapableHandleScope.Escape(instance);
}

llvm::AnyCoroSuspendInst* AnyCoroSuspendInstWrapper::getAnyCoroSuspendInst() {
    return static_cast<llvm::AnyCoroSuspendInst*>(getValue());
}

Nan::Persistent<v8::FunctionTemplate>& AnyCoroSuspendInstWrapper::anyCoroSuspendInstTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> tmpl {};

    if (tmpl.IsEmpty()) {
        auto anyCoroIdInstWrapperTemplate = Nan::New<v8::FunctionTemplate>(AnyCoroSuspendInstWrapper::New);
        anyCoroSuspendInstWrapperTemplate->SetClassName(Nan::New("AnyCoroSuspendInst").ToLocalChecked());
        anyCoroSuspendInstWrapperTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        anyCoroSuspendInstWrapperTemplate->Inherit(Nan::New(intrinsicTemplate()));

        Nan::SetPrototypeMethod(anyCoroSuspendInstWrapperTemplate, "getCoroSave", getCoroSave);

        tmpl.Reset(anyCoroIdInstWrapperTemplate);
    }
}