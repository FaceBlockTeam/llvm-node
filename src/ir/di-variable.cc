#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "di-variable.h"
#include "di-scope.h"
#include "di-file.h"
#include "di-type.h"

NAN_MODULE_INIT(DIVariableWrapper::Init) {
    auto diVariable = Nan::GetFunction(Nan::New(diVariableTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("DIVariable").ToLocalChecked(), diVariable);
}

llvm::DIVariable *DIVariableWrapper::getDIVariable() {
    return diVariable;
}

v8::Local<v8::Object> DIVariableWrapper::of(llvm::DIVariable * diVariable) {
    auto constructorFunction = Nan::GetFunction(Nan::New(diVariableTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(diVariable) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe;
    return escapeScpoe.Escape(instance);
}

bool DIVariableWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(diVariableTemplate())->HasInstance(value);
}

DIVariableWrapper::DIVariableWrapper(llvm::DIVariable *variable): diVariable(variable) {}
    
NAN_METHOD(DIVariableWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("DIVariable constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *diVariable = static_cast<llvm::DIVariable*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DIVariableWrapper(diVariable);
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(DIVariableWrapper::getLine) {
    if (info.Length() != 0) {
        return Nan::ThrowTypeError("No argument is required");
    }

    auto *diVariable = DIVariableWrapper::FromValue(info.Holder())->getDIVariable();
    uint32_t line = diVariable->getLine();
    info.GetReturnValue().Set(line);
}

NAN_METHOD(DIVariableWrapper::getScope) {
    if (info.Length() != 0) {
        return Nan::ThrowTypeError("No argument is required");
    }

    auto *diVariable = DIVariableWrapper::FromValue(info.Holder())->getDIVariable();
    auto *diScope = diVariable->getScope();
    info.GetReturnValue().Set(DIScopeWrapper::of(diScope));
}

NAN_METHOD(DIVariableWrapper::getName) {
    if (info.Length() != 0) {
        return Nan::ThrowTypeError("No argument is required");
    }

    auto *diVariable = DIVariableWrapper::FromValue(info.Holder())->getDIVariable();
    std::string name = diVariable->getName().str();
    info.GetReturnValue().Set(Nan::New(name).ToLocalChecked());
}

NAN_METHOD(DIVariableWrapper::getFile) {
    if (info.Length() != 0) {
        return Nan::ThrowTypeError("No argument is required");
    }

    auto *diVariable = DIVariableWrapper::FromValue(info.Holder())->getDIVariable();
    auto *diFile = diVariable->getFile();
    info.GetReturnValue().Set(DIFileWrapper::of(diFile));
}

NAN_METHOD(DIVariableWrapper::getType) {
    if (info.Length() != 0) {
        return Nan::ThrowTypeError("No argument is required");
    }

    auto *diVariable = DIVariableWrapper::FromValue(info.Holder())->getDIVariable();
    auto *diType = diVariable->getType();
    info.GetReturnValue().Set(DITypeWrapper::of(diType));
}

NAN_METHOD(DIVariableWrapper::getFilename) {
    if (info.Length() != 0) {
        return Nan::ThrowTypeError("No argument is required");
    }

    auto *diVariable = DIVariableWrapper::FromValue(info.Holder())->getDIVariable();
    std::string filename = diVariable->getFilename().str();
    info.GetReturnValue().Set(Nan::New(filename).ToLocalChecked());
}

NAN_METHOD(DIVariableWrapper::getDirectory) {
    if (info.Length() != 0) {
        return Nan::ThrowTypeError("No argument is required");
    }

    auto *diVariable = DIVariableWrapper::FromValue(info.Holder())->getDIVariable();
    std::string dir = diVariable->getDirectory().str();
    info.GetReturnValue().Set(Nan::New(dir).ToLocalChecked());
}

NAN_METHOD(DIVariableWrapper::getSizeInBits) {
    if (info.Length() != 0) {
        return Nan::ThrowTypeError("No argument is required");
    }

    auto *diVariable = DIVariableWrapper::FromValue(info.Holder())->getDIVariable();
    uint32_t sizeInBits = diVariable->getSizeInBits().getValue();
    info.GetReturnValue().Set(sizeInBits);
}

Nan::Persistent<v8::FunctionTemplate> &DIVariableWrapper::diVariableTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(DIVariableWrapper::New);
        localTemplate->SetClassName(Nan::New("DIVariable").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        Nan::SetPrototypeMethod(localTemplate, "getLine", DIVariableWrapper::getLine);
        Nan::SetPrototypeMethod(localTemplate, "getScope", DIVariableWrapper::getScope);
        Nan::SetPrototypeMethod(localTemplate, "getName", DIVariableWrapper::getName);
        Nan::SetPrototypeMethod(localTemplate, "getFile", DIVariableWrapper::getFile);
        Nan::SetPrototypeMethod(localTemplate, "getType", DIVariableWrapper::getType);
        Nan::SetPrototypeMethod(localTemplate, "getFilename", DIVariableWrapper::getFilename);
        Nan::SetPrototypeMethod(localTemplate, "getDirectory", DIVariableWrapper::getDirectory);
        Nan::SetPrototypeMethod(localTemplate, "getSizeInBits", DIVariableWrapper::getSizeInBits);
        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}