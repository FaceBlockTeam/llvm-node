#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "di-subprogram.h"

NAN_MODULE_INIT(DISubprogramWrapper::Init) {
    auto diSubprogram = Nan::GetFunction(Nan::New(diSubprogramTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("DISubprogram").ToLocalChecked(), diSubprogram);
}

v8::Local<v8::Object> DISubprogramWrapper::of(llvm::DISubprogram *diSubprogram) {
    auto constructorFunction = Nan::GetFunction(Nan::New(diSubprogramTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(diSubprogram) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe;
    return escapeScpoe.Escape(instance);
}

llvm::DISubprogram *DISubprogramWrapper::getDISubprogram() {
    return diSubprogram;
}

bool DISubprogramWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(diSubprogramTemplate())->HasInstance(value);
}

DISubprogramWrapper::DISubprogramWrapper(llvm::DISubprogram *subprogram): diSubprogram(subprogram) {}

NAN_METHOD(DISubprogramWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("DISubprogram constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *diSubprogram = static_cast<llvm::DISubprogram*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DISubprogramWrapper(diSubprogram);
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

Nan::Persistent<v8::FunctionTemplate> &DISubprogramWrapper::diSubprogramTemplate() {
    Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(DISubprogramWrapper::New);
        localTemplate->SetClassName(Nan::New("DISubprogram").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}