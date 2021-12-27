#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "di-subroutine-type.h"

NAN_MODULE_INIT(DISubroutineTypeWrapper::Init) {
    auto diSubroutineType = Nan::GetFunction(Nan::New(diSubroutineTypeTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("DISubroutineType").ToLocalChecked(), diSubroutineType);
}

llvm::DISubroutineType *DISubroutineTypeWrapper::getDISubroutineType() {
    return diSubroutineType;
}

v8::Local<v8::Object> DISubroutineTypeWrapper::of(llvm::DISubroutineType *diSubroutineType) {
    auto constructorFunction = Nan::GetFunction(Nan::New(diSubroutineTypeTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(diSubroutineType) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe;
    return escapeScpoe.Escape(instance);
}

bool DISubroutineTypeWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(diSubroutineTypeTemplate())->HasInstance(value);
}

DISubroutineTypeWrapper::DISubroutineTypeWrapper(llvm::DISubroutineType *subroutineType): diSubroutineType(subroutineType) {}

NAN_METHOD(DISubroutineTypeWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("DISubroutineType constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *diSubroutineType = static_cast<llvm::DISubroutineType*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DISubroutineTypeWrapper(diSubroutineType);
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

Nan::Persistent<v8::FunctionTemplate> &DISubroutineTypeWrapper::diSubroutineTypeTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(DISubroutineTypeWrapper::New);
        localTemplate->SetClassName(Nan::New("DISubroutineType").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}