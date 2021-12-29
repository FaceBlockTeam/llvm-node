#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "di-type-ref-array.h"

NAN_MODULE_INIT(DITypeRefArrayWrapper::Init) {
    auto diTypeRefArray = Nan::GetFunction(Nan::New(diTypeRefArrayTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("DITypeRefArray").ToLocalChecked(), diTypeRefArray);
}

llvm::DITypeRefArray DITypeRefArrayWrapper::getDITypeRefArray() {
    return diTypeRefArray;
}

v8::Local<v8::Object> DITypeRefArrayWrapper::of(llvm::DITypeRefArray diTypeRefArray) {
    auto constructorFunction = Nan::GetFunction(Nan::New(diTypeRefArrayTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(&diTypeRefArray) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe;
    return escapeScpoe.Escape(instance);
}

bool DITypeRefArrayWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(diTypeRefArrayTemplate())->HasInstance(value);
}

DITypeRefArrayWrapper::DITypeRefArrayWrapper(llvm::DITypeRefArray typeRefArray): diTypeRefArray(typeRefArray) {}

NAN_METHOD(DITypeRefArrayWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("DITypeRefArray constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *diTypeRefArray = static_cast<llvm::DITypeRefArray*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DITypeRefArrayWrapper(*diTypeRefArray);
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

Nan::Persistent<v8::FunctionTemplate> &DITypeRefArrayWrapper::diTypeRefArrayTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(DITypeRefArrayWrapper::New);
        localTemplate->SetClassName(Nan::New("DITypeRefArray").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}