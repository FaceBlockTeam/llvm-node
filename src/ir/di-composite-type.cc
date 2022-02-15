#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "di-composite-type.h"

NAN_MODULE_INIT(DICompositeTypeWrapper::Init) {
    auto diCompositeType = Nan::GetFunction(Nan::New(diCompositeTypeTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("DICompositeType").ToLocalChecked(), diCompositeType);
}

llvm::DICompositeType *DICompositeTypeWrapper::getDICompositeType() {
    return diCompositeType;
}

v8::Local<v8::Object> DICompositeTypeWrapper::of(llvm::DICompositeType *diCompositeType) {
    auto constructorFunction = Nan::GetFunction(Nan::New(diCompositeTypeTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(diCompositeType) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe;
    return escapeScpoe.Escape(instance);
}

bool DICompositeTypeWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(diCompositeTypeTemplate())->HasInstance(value);
}

DICompositeTypeWrapper::DICompositeTypeWrapper(llvm::DICompositeType *compositeType): diCompositeType(compositeType) {}

NAN_METHOD(DICompositeTypeWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("DICompositeType constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *diCompositeType = static_cast<llvm::DICompositeType*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DICompositeTypeWrapper(diCompositeType);
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

Nan::Persistent<v8::FunctionTemplate> &DICompositeTypeWrapper::diCompositeTypeTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(DICompositeTypeWrapper::New);
        localTemplate->SetClassName(Nan::New("DICompositeType").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}