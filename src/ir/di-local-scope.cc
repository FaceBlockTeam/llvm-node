#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "di-local-scope.h"
#include "di-subprogram.h"

NAN_MODULE_INIT(DILocalScopeWrapper::Init) {
    auto diLocalScope = Nan::GetFunction(Nan::New(diLocalScopeTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("DILocalScope").ToLocalChecked(), diLocalScope);
}

llvm::DILocalScope *DILocalScopeWrapper::getDILocalScope() {
    return diLocalScope;
}

v8::Local<v8::Object> DILocalScopeWrapper::of(llvm::DILocalScope *diLocalScope) {
    auto constructorFunction = Nan::GetFunction(Nan::New(diLocalScopeTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(diLocalScope) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe;
    return escapeScpoe.Escape(instance);
}

bool DILocalScopeWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(diLocalScopeTemplate())->HasInstance(value);
}

DILocalScopeWrapper::DILocalScopeWrapper(llvm::DILocalScope *localScope): diLocalScope(localScope) {}


NAN_METHOD(DILocalScopeWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("DILocalScope constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *diLocalScope = static_cast<llvm::DILocalScope*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DILocalScopeWrapper(diLocalScope);
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(DILocalScopeWrapper::getSubprogram) {
    if (info.Length() != 0) {
        return Nan::ThrowTypeError("No argument is required");
    }
    auto *diLocalScope = DILocalScopeWrapper::FromValue(info.Holder())->getDILocalScope();
    auto *subprogram = diLocalScope->getSubprogram();
    info.GetReturnValue().Set(DISubprogramWrapper::of(subprogram));
}

Nan::Persistent<v8::FunctionTemplate> &DILocalScopeWrapper::diLocalScopeTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(DILocalScopeWrapper::New);
        localTemplate->SetClassName(Nan::New("DILocalScope").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        Nan::SetPrototypeMethod(localTemplate, "getSubprogram", DILocalScopeWrapper::getSubprogram);
        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}