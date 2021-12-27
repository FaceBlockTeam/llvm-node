#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "di-expression.h"

NAN_MODULE_INIT(DIExpressionWrapper::Init) {
    auto diExpression= Nan::GetFunction(Nan::New(diExpressionTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("DIExpression").ToLocalChecked(), diExpression);
}

v8::Local<v8::Object> DIExpressionWrapper::of(llvm::DIExpression *diExpression) {
    auto constructorFunction = Nan::GetFunction(Nan::New(diExpressionTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(diExpression) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe;
    return escapeScpoe.Escape(instance);
}

DIExpressionWrapper::DIExpressionWrapper(llvm::DIExpression *expression): diExpression(expression) {}

NAN_METHOD(DIExpressionWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("DIExpression constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *diExpression = static_cast<llvm::DIExpression*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DIExpressionWrapper(diExpression);
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

Nan::Persistent<v8::FunctionTemplate> &DIExpressionWrapper::diExpressionTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(DIExpressionWrapper::New);
        localTemplate->SetClassName(Nan::New("DIExpression").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}