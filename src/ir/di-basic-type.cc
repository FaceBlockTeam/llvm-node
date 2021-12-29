#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "di-basic-type.h"

NAN_MODULE_INIT(DIBasicTypeWrapper::Init) {
    auto diBasicType = Nan::GetFunction(Nan::New(diBasicTypeTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("DIBasicType").ToLocalChecked(), diBasicType);
}

llvm::DIBasicType *DIBasicTypeWrapper::getDIBasicType() {
    return diBasicType;
}

v8::Local<v8::Object> DIBasicTypeWrapper::of(llvm::DIBasicType *diBasicType) {
    auto constructorFunction = Nan::GetFunction(Nan::New(diBasicTypeTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(diBasicType) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe;
    return escapeScpoe.Escape(instance);
}

bool DIBasicTypeWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(diBasicTypeTemplate())->HasInstance(value);
}

DIBasicTypeWrapper::DIBasicTypeWrapper(llvm::DIBasicType *BasicType): diBasicType(BasicType) {}

NAN_METHOD(DIBasicTypeWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("DIBasicType constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *diBasicType = static_cast<llvm::DIBasicType*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DIBasicTypeWrapper(diBasicType);
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

Nan::Persistent<v8::FunctionTemplate> &DIBasicTypeWrapper::diBasicTypeTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(DIBasicTypeWrapper::New);
        localTemplate->SetClassName(Nan::New("DIBasicType").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}