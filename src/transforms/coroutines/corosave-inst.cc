#include <nan.h>
#include "Transforms/Coroutines/CoroInstr.h"
#include "corosave-inst.h"

NAN_MODULE_INIT(CoroSaveInstWrapper::Init) {
    auto coroSaveInst = Nan::GetFunction(Nan::New(coroSaveInstTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("CoroSaveInst").ToLocalChecked(), coroSaveInst);
}

NAN_METHOD(CoroSaveInstWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *coroSaveInst = static_cast<llvm::CoroSaveInst*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new CoroSaveInstWrapper { coroSaveInst };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> CoroSaveInstWrapper::of(llvm::CoroSaveInst* coroSaveInst) {
    auto constructorFunction = Nan::GetFunction(Nan::New(coroSaveInstTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(coroSaveInst)};

    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapableHandleScope {};
    return escapableHandleScope.Escape(instance);
}

llvm::CoroSaveInst* CoroSaveInstWrapper::getCoroSaveInst() {
    return static_cast<llvm::CoroSaveInst*>(getValue());
}

Nan::Persistent<v8::FunctionTemplate>& CoroSaveInstWrapper::coroSaveInstTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> tmpl {};

    if (tmpl.IsEmpty()) {
        auto coroSaveInstWrapperTemplate = Nan::New<v8::FunctionTemplate>(CoroSaveInstWrapper::New);
        coroSaveInstWrapperTemplate->SetClassName(Nan::New("CoroSaveInst").ToLocalChecked());
        coroSaveInstWrapperTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        coroSaveInstWrapperTemplate->Inherit(Nan::New(intrinsicTemplate()));

        tmpl.Reset(coroSaveInstWrapperTemplate);
    }
}
