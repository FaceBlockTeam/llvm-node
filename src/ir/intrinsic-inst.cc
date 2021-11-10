#include <nan.h>
#include <llvm/IR/IntrinsicInst.h>
#include "intrinsic-inst.h"
#include "value.h"

NAN_MODULE_INIT(IntrinsicInstWrapper::Init) {
    auto intrinsicInst = Nan::GetFunction(Nan::New(intrinsicInstTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("IntrinsicInst").ToLocalChecked(), intrinsicInst);
}

v8::Local<v8::Object> IntrinsicInstWrapper::of(llvm::IntrinsicInst* intrinsicInst) {
    auto constructorFunction = Nan::GetFunction(Nan::New(intrinsicInstTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(intrinsicInst)};

    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapableHandleScope {};
    return escapableHandleScope.Escape(instance);
}

llvm::IntrinsicInst* IntrinsicInstWrapper::getIntrinsicInst() {
    return static_cast<llvm::IntrinsicInst*>(getValue());
}

NAN_METHOD(IntrinsicInstWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *intrinsicInst = static_cast<llvm::IntrinsicInst*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new IntrinsicInstWrapper { intrinsicInst };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(IntrinsicInstWrapper::getIntrinsicID) {
    auto* intrinsicInst = IntrinsicInstWrapper::FromValue(info.Holder())->getIntrinsicInst();
    info.GetReturnValue().Set(intrinsicInst->getIntrinsicID());
}

Nan::Persistent<v8::FunctionTemplate>& IntrinsicInstWrapper::intrinsicInstTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> tmpl {};

    if (tmpl.IsEmpty()) {
        auto intrinsicInstWrapperTemplate = Nan::New<v8::FunctionTemplate>(IntrinsicInstWrapper::New);
        intrinsicInstWrapperTemplate->SetClassName(Nan::New("IntrinsicInst").ToLocalChecked());
        intrinsicInstWrapperTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        intrinsicInstWrapperTemplate->Inherit(Nan::New(valueTemplate()));

        Nan::SetPrototypeMethod(intrinsicInstWrapperTemplate, "getIntrinsicID", IntrinsicInstWrapper::getIntrinsicID);

        tmpl.Reset(intrinsicInstWrapperTemplate);
    }

    return tmpl;
}