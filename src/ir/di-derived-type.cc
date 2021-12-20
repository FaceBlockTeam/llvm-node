#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "di-derived-type.h"

NAN_MODULE_INIT(DIDerivedTypeWrapper::Init) {
    auto diDerivedType = Nan::GetFunction(Nan::New(diDerivedTypeTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("DIDerivedType").ToLocalChecked(), diDerivedType);
}

v8::Local<v8::Object> DIDerivedTypeWrapper::of(llvm::DIDerivedType *diDerivedType) {
    auto constructorFunction = Nan::GetFunction(Nan::New(diDerivedTypeTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(diDerivedType) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe;
    return escapeScpoe.Escape(instance);
}

DIDerivedTypeWrapper::DIDerivedTypeWrapper(llvm::DIDerivedType *derivedType): diDerivedType(derivedType) {}

NAN_METHOD(DIDerivedTypeWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("DIDerivedType constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *diDerivedType = static_cast<llvm::DIDerivedType*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DIDerivedTypeWrapper(diDerivedType);
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

Nan::Persistent<v8::FunctionTemplate> &DIDerivedTypeWrapper::diDerivedTypeTemplate() {
    Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(DIDerivedTypeWrapper::New);
        localTemplate->SetClassName(Nan::New("DIDerivedType").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}