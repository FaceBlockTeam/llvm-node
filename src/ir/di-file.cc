#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "di-file.h"

NAN_MODULE_INIT(DIFileWrapper::Init) {
    auto diFile = Nan::GetFunction(Nan::New(diFileTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("DIFile").ToLocalChecked(), diFile);
}

NAN_METHOD(DIFileWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("DIFile constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *diFile = static_cast<llvm::DIFile*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DIFileWrapper(diFile);
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> DIFileWrapper::of(llvm::DIFile *diFile) {
    auto constructorFunction = Nan::GetFunction(Nan::New(diFileTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(diFile) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe;
    return escapeScpoe.Escape(instance);
}

llvm::DIFile *DIFileWrapper::getDIFile() {
    return static_cast<llvm::DIFile*>(DIScopeWrapper::getDIScope());
}

bool DIFileWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(diFileTemplate())->HasInstance(value);
} 

DIFileWrapper::DIFileWrapper(llvm::DIFile *file): DIScopeWrapper(file) {}

Nan::Persistent<v8::FunctionTemplate> &DIFileWrapper::diFileTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(DIFileWrapper::New);
        localTemplate->SetClassName(Nan::New("DIFile").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        localTemplate->Inherit(Nan::New(DIScopeWrapper::diScopeTemplate()));
        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}