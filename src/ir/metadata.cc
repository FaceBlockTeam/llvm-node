#include <nan.h>
#include "metadata.h"

NAN_MODULE_INIT(MetadataWrapper::Init) {
    auto diType = Nan::GetFunction(Nan::New(metadataTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("Metadata").ToLocalChecked(), diType);
}

llvm::Metadata *MetadataWrapper::getMetadata() {
    return metadata;
}

bool MetadataWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(metadataTemplate())->HasInstance(value);
}

MetadataWrapper::MetadataWrapper(llvm::Metadata *data): metadata(data) {}

NAN_METHOD(MetadataWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Metadata constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *metadata = static_cast<llvm::Metadata*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new MetadataWrapper(metadata);
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

Nan::Persistent<v8::FunctionTemplate> &MetadataWrapper::metadataTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(MetadataWrapper::New);
        localTemplate->SetClassName(Nan::New("Metadata").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}