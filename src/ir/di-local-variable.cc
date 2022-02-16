#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "di-local-variable.h"
#include "di-local-scope.h"

NAN_MODULE_INIT(DILocalVariableWrapper::Init) {
    auto diLocalVariable = Nan::GetFunction(Nan::New(diLocalVariableTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("DILocalVariable").ToLocalChecked(), diLocalVariable);
}

llvm::DILocalVariable *DILocalVariableWrapper::getDILocalVariable() {
    return static_cast<llvm::DILocalVariable*>(DIVariableWrapper::getDIVariable());
}

v8::Local<v8::Object> DILocalVariableWrapper::of(llvm::DILocalVariable *diLocalVariable) {
    auto constructorFunction = Nan::GetFunction(Nan::New(diLocalVariableTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(diLocalVariable) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe;
    return escapeScpoe.Escape(instance);
}

bool DILocalVariableWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(diLocalVariableTemplate())->HasInstance(value);
}

DILocalVariableWrapper::DILocalVariableWrapper(llvm::DILocalVariable *localVariable): DIVariableWrapper(localVariable) {}
    
NAN_METHOD(DILocalVariableWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("DILocalVariable constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *diLocalVariable = static_cast<llvm::DILocalVariable*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DILocalVariableWrapper(diLocalVariable);
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(DILocalVariableWrapper::getScope) {
    if (info.Length() != 0) {
        return Nan::ThrowTypeError("No argument is required");
    }

    auto *diLocalVariable = DILocalVariableWrapper::FromValue(info.Holder())->getDILocalVariable();
    auto *diLocalScope = diLocalVariable->getScope();
    info.GetReturnValue().Set(DILocalScopeWrapper::of(diLocalScope));
}

NAN_METHOD(DILocalVariableWrapper::isParameter) {
    if (info.Length() != 0) {
        return Nan::ThrowTypeError("No argument is required");
    }

    auto *diLocalVariable = DILocalVariableWrapper::FromValue(info.Holder())->getDILocalVariable();
    bool isParameter = diLocalVariable->isParameter();
    info.GetReturnValue().Set(isParameter);
}

Nan::Persistent<v8::FunctionTemplate> &DILocalVariableWrapper::diLocalVariableTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(DILocalVariableWrapper::New);
        localTemplate->SetClassName(Nan::New("DILocalVariable").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        localTemplate->Inherit(Nan::New(DIVariableWrapper::diVariableTemplate()));
        Nan::SetPrototypeMethod(localTemplate, "getScope", DILocalVariableWrapper::getScope);
        Nan::SetPrototypeMethod(localTemplate, "isParameter", DILocalVariableWrapper::isParameter);
        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}