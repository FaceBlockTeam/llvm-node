#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "di-node-array.h"

NAN_MODULE_INIT(DINodeArrayWrapper::Init) {
    auto diNodeArray = Nan::GetFunction(Nan::New(diNodeArrayTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("DINodeArray").ToLocalChecked(), diNodeArray);
}

llvm::DINodeArray DINodeArrayWrapper::getDINodeArray() {
    return diNodeArray;
}

v8::Local<v8::Object> DINodeArrayWrapper::of(llvm::DINodeArray diNodeArray) {
    auto constructorFunction = Nan::GetFunction(Nan::New(diNodeArrayTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(&diNodeArray) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe;
    return escapeScpoe.Escape(instance);
}

bool DINodeArrayWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(diNodeArrayTemplate())->HasInstance(value);
}

DINodeArrayWrapper::DINodeArrayWrapper(llvm::DINodeArray nodeArray): diNodeArray(nodeArray) {}

NAN_METHOD(DINodeArrayWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("DINodeArray constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *diNodeArray = static_cast<llvm::DINodeArray*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DINodeArrayWrapper(*diNodeArray);
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

Nan::Persistent<v8::FunctionTemplate> &DINodeArrayWrapper::diNodeArrayTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(DINodeArrayWrapper::New);
        localTemplate->SetClassName(Nan::New("DINodeArray").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}