#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "di-scope.h"

NAN_MODULE_INIT(DIScopeWrapper::Init) {
    auto diScope = Nan::GetFunction(Nan::New(diScopeTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("DIScope").ToLocalChecked(), diScope);
}

NAN_METHOD(DIScopeWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("DIScope constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *diScope = static_cast<llvm::DIScope*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DIScopeWrapper(diScope);
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}


v8::Local<v8::Object> DIScopeWrapper::of(llvm::DIScope *diScope) {
    auto constructorFunction = Nan::GetFunction(Nan::New(diScopeTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(diScope) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe;
    return escapeScpoe.Escape(instance);
}

llvm::DIScope *DIScopeWrapper::getDIScope() {
    return diScope;
}

bool DIScopeWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(diScopeTemplate())->HasInstance(value);
} 

DIScopeWrapper::DIScopeWrapper(llvm::DIScope *scope): diScope(scope) {}

Nan::Persistent<v8::FunctionTemplate> &DIScopeWrapper::diScopeTemplate() {
    Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(DIScopeWrapper::New);
        localTemplate->SetClassName(Nan::New("DIScope").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}

NAN_METHOD(DIScopeWrapper::getFile) {

}

NAN_METHOD(DIScopeWrapper::getFilename) {

}


NAN_METHOD(DIScopeWrapper::getDirectory) {
    
}

NAN_METHOD(DIScopeWrapper::getName) {

}