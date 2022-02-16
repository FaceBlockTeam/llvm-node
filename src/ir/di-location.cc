#include <nan.h>
#include "llvm-context.h"
#include "di-location.h"
#include "di-local-scope.h"

NAN_MODULE_INIT(DILocationWrapper::Init) {
    auto diLocation = Nan::GetFunction(Nan::New(diLocationTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("DILocation").ToLocalChecked(), diLocation);
}

llvm::DILocation *DILocationWrapper::getDILocation() {
    return diLocation;
}

v8::Local<v8::Object> DILocationWrapper::of(llvm::DILocation *diLocation) {
    auto constructorFunction = Nan::GetFunction(Nan::New(diLocationTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(diLocation) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe;
    return escapeScpoe.Escape(instance);
}

bool DILocationWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(diLocationTemplate())->HasInstance(value);
}


DILocationWrapper::DILocationWrapper(llvm::DILocation *location): diLocation(location) {}
    
NAN_METHOD(DILocationWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("DILocation constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *diLocation = static_cast<llvm::DILocation*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DILocationWrapper(diLocation);
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(DILocationWrapper::get) {
    if (info.Length() != 4 || !LLVMContextWrapper::isInstance(info[0]) || !info[1]->IsInt32()
        || !info[2]->IsInt32() || !DILocalScopeWrapper::isInstance(info[3])) {
            return Nan::ThrowTypeError("get accepts only 4 arguments");
    }
    
    auto &context = LLVMContextWrapper::FromValue(info[0])->getContext();
    uint32_t line = Nan::To<uint32_t>(info[1]).FromJust();
    uint32_t column = Nan::To<uint32_t>(info[2]).FromJust();
    auto *diLocalScope = DILocalScopeWrapper::FromValue(info[3])->getDILocalScope();
    auto *diLocation = llvm::DILocation::get(context, line, column, diLocalScope);

    info.GetReturnValue().Set(DILocationWrapper::of(diLocation));
}

Nan::Persistent<v8::FunctionTemplate> &DILocationWrapper::diLocationTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(DILocationWrapper::New);
        localTemplate->SetClassName(Nan::New("DILocation").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        auto function = Nan::GetFunction(localTemplate).ToLocalChecked();
        Nan::SetMethod(function, "get", DILocationWrapper::get);
        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}