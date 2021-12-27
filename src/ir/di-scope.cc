#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "di-scope.h"
#include "di-file.h"

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
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(DIScopeWrapper::New);
        localTemplate->SetClassName(Nan::New("DIScope").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        Nan::SetPrototypeMethod(localTemplate, "getFile", DIScopeWrapper::getFile);
        Nan::SetPrototypeMethod(localTemplate, "getFilename", DIScopeWrapper::getFilename);
        Nan::SetPrototypeMethod(localTemplate, "getDirectory", DIScopeWrapper::getDirectory);
        Nan::SetPrototypeMethod(localTemplate, "getName", DIScopeWrapper::getName);
        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}

NAN_METHOD(DIScopeWrapper::getFile) {
    if (info.Length() != 0) {
        return Nan::ThrowTypeError("No argument is required");
    }

    auto *diScope = DIScopeWrapper::FromValue(info.Holder())->getDIScope();
    auto *diFile = diScope->getFile();
    info.GetReturnValue().Set(DIFileWrapper::of(diFile));
}

NAN_METHOD(DIScopeWrapper::getFilename) {
    if (info.Length() != 0) {
        return Nan::ThrowTypeError("No argument is required");
    }

    auto *diScope = DIScopeWrapper::FromValue(info.Holder())->getDIScope();
    std::string filename = diScope->getFilename().str();
    info.GetReturnValue().Set(Nan::New(filename).ToLocalChecked());
}


NAN_METHOD(DIScopeWrapper::getDirectory) {
    if (info.Length() != 0) {
        return Nan::ThrowTypeError("No argument is required");
    }

    auto *diScope = DIScopeWrapper::FromValue(info.Holder())->getDIScope();
    std::string directory = diScope->getDirectory().str();
    info.GetReturnValue().Set(Nan::New(directory).ToLocalChecked());
}

NAN_METHOD(DIScopeWrapper::getName) {
    if (info.Length() != 0) {
        return Nan::ThrowTypeError("No argument is required");
    }

    auto *diScope = DIScopeWrapper::FromValue(info.Holder())->getDIScope();
    std::string name = diScope->getName().str();
    info.GetReturnValue().Set(Nan::New(name).ToLocalChecked());
}