#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "di-compile-unit.h"

NAN_MODULE_INIT(DICompileUnitWrapper::Init) {
    auto diCompileUnit = Nan::GetFunction(Nan::New(diCompileUnitTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("DICompileUnit").ToLocalChecked(), diCompileUnit);
}

NAN_METHOD(DICompileUnitWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("DICompileUnit constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *diCompileUnit = static_cast<llvm::DICompileUnit*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DICompileUnitWrapper(diCompileUnit);
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> DICompileUnitWrapper::of(llvm::DICompileUnit *diCompileUnit) {
    auto constructorFunction = Nan::GetFunction(Nan::New(diCompileUnitTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(diCompileUnit) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe;
    return escapeScpoe.Escape(instance);
}

llvm::DICompileUnit *DICompileUnitWrapper::getDICompileUnit() {
    return diCompileUnit;
}

bool DICompileUnitWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(diCompileUnitTemplate())->HasInstance(value);
} 

DICompileUnitWrapper::DICompileUnitWrapper(llvm::DICompileUnit *cu): diCompileUnit(cu) {}

Nan::Persistent<v8::FunctionTemplate> &DICompileUnitWrapper::diCompileUnitTemplate() {
    Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(DICompileUnitWrapper::New);
        localTemplate->SetClassName(Nan::New("DICompileUnit").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}