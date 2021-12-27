#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "di-module.h"

NAN_MODULE_INIT(DIModuleWrapper::Init) {
    auto diModule = Nan::GetFunction(Nan::New(diModuleTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("DIModule").ToLocalChecked(), diModule);
}

NAN_METHOD(DIModuleWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("DIModule constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *diModule = static_cast<llvm::DIModule*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DIModuleWrapper(diModule);
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}


v8::Local<v8::Object> DIModuleWrapper::of(llvm::DIModule *diModule) {
    auto constructorFunction = Nan::GetFunction(Nan::New(diModuleTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(diModule) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe;
    return escapeScpoe.Escape(instance);
}

DIModuleWrapper::DIModuleWrapper(llvm::DIModule *module): diModule(module) {}

Nan::Persistent<v8::FunctionTemplate> &DIModuleWrapper::diModuleTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(DIModuleWrapper::New);
        localTemplate->SetClassName(Nan::New("DIModule").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}