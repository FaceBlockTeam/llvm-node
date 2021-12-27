#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "di-type.h"
#include "di-scope.h"

NAN_MODULE_INIT(DITypeWrapper::Init) {
    auto diType = Nan::GetFunction(Nan::New(diTypeTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("DIType").ToLocalChecked(), diType);
}

v8::Local<v8::Object> DITypeWrapper::of(llvm::DIType *diType) {
    auto constructorFunction = Nan::GetFunction(Nan::New(diTypeTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(diType) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe;
    return escapeScpoe.Escape(instance);
}

llvm::DIType *DITypeWrapper::getDIType() {
    return diType;
}

bool DITypeWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(diTypeTemplate())->HasInstance(value);
}

DITypeWrapper::DITypeWrapper(llvm::DIType *type): diType(type) {}

NAN_METHOD(DITypeWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("DIType constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *diType = static_cast<llvm::DIType*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DITypeWrapper(diType);
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(DITypeWrapper::getLine) {
    if (info.Length() != 0) {
        return Nan::ThrowTypeError("No argument should be provided");
    }

    auto *diType = DITypeWrapper::FromValue(info.Holder())->getDIType();
    uint32_t line = diType->getLine();
    info.GetReturnValue().Set(line);
}

NAN_METHOD(DITypeWrapper::getScope) {
    if (info.Length() != 0) {
        return Nan::ThrowTypeError("No argument should be provided");
    }

    auto *diType = DITypeWrapper::FromValue(info.Holder())->getDIType();
    auto *diScope = diType->getScope();
    info.GetReturnValue().Set(DIScopeWrapper::of(diScope));
}

NAN_METHOD(DITypeWrapper::getName) {
    if (info.Length() != 0) {
        return Nan::ThrowTypeError("No argument should be provided");
    }

    auto *diType = DITypeWrapper::FromValue(info.Holder())->getDIType();
    std::string name = diType->getName().str();
    info.GetReturnValue().Set(Nan::New(name).ToLocalChecked());
}

Nan::Persistent<v8::FunctionTemplate> &DITypeWrapper::diTypeTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(DITypeWrapper::New);
        localTemplate->SetClassName(Nan::New("DIType").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        Nan::SetPrototypeMethod(localTemplate, "getLine", DITypeWrapper::getLine);
        Nan::SetPrototypeMethod(localTemplate, "getScope", DITypeWrapper::getScope);
        Nan::SetPrototypeMethod(localTemplate, "getName", DITypeWrapper::getName);
        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}