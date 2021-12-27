#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "di-flags.h"

NAN_MODULE_INIT(DIFlagsWrapper::Init) {
    auto diFlags = Nan::GetFunction(Nan::New(diFlagsTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("DIFlags").ToLocalChecked(), diFlags);
}

llvm::DINode::DIFlags DIFlagsWrapper::getDIFlags() {
    return diFlags;
}

v8::Local<v8::Object> DIFlagsWrapper::of(llvm::DINode::DIFlags diFlags) {
    auto constructorFunction = Nan::GetFunction(Nan::New(diFlagsTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(&diFlags) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe;
    return escapeScpoe.Escape(instance);
}

bool DIFlagsWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(diFlagsTemplate())->HasInstance(value);
}

DIFlagsWrapper::DIFlagsWrapper(llvm::DINode::DIFlags flags): diFlags(flags) {}

NAN_METHOD(DIFlagsWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("DIFlags constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *diFlags = static_cast<llvm::DINode::DIFlags*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DIFlagsWrapper(*diFlags);
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

Nan::Persistent<v8::FunctionTemplate> &DIFlagsWrapper::diFlagsTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(DIFlagsWrapper::New);
        localTemplate->SetClassName(Nan::New("DIFlags").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}