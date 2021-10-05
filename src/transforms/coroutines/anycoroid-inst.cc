#include <nan.h>
#include "llvm/Transforms/Coroutines/CoroInstr.h"
#include "anycoroid-inst.h"
#include "../../ir/intrinsic-inst.h"

NAN_MODULE_INIT(AnyCoroIdInstWrapper::Init) {
    auto anyCoroIdInst = Nan::GetFunction(Nan::New(anyCoroIdInstTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("AnyCoroIdInst").ToLocalChecked(), anyCoroIdInst);
}

v8::Local<v8::Object> AnyCoroIdInstWrapper::of(llvm::AnyCoroIdInst* anyCoroIdInst) {
    auto constructorFunction = Nan::GetFunction(Nan::New(anyCoroIdInstTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(anyCoroIdInst)};

    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapableHandleScope {};
    return escapableHandleScope.Escape(instance);
}

llvm::AnyCoroIdInst* AnyCoroIdInstWrapper::getAnyCoroIdInst() {
    return static_cast<llvm::AnyCoroIdInst*>(getValue());
}

NAN_METHOD(AnyCoroIdInstWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *anyCoroIdInst = static_cast<llvm::AnyCoroIdInst*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new AnyCoroIdInstWrapper { anyCoroIdInst };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(AnyCoroIdInstWrapper::getCoroAlloc) {
    auto *anyCoroIdInst = AnyCoroIdInstWrapper::FromValue(info.Holder())->getAnyCoroIdInst();
    auto *coroAllocInst = anyCoroIdInst->getCoroAlloc();
    info.GetReturnValue().Set(CoroAllocInstWrapper::of(coroAllocInst));
}

NAN_METHOD(AnyCoroIdInstWrapper::getCoroBegin) {
    auto *anyCoroIdInst = AnyCoroIdInstWrapper::FromValue(info.Holder())->getAnyCoroIdInst();
    auto *intrinsicInst = anyCoroIdInst->getCoroBegin();
    info.GetReturnValue().Set(IntrinsicInstWrapper::of(intrinsicInst));
}

Nan::Persistent<v8::FunctionTemplate>& AnyCoroIdInstWrapper::anyCoroIdInstTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> tmpl {};

    if (tmpl.IsEmpty()) {
        auto anyCoroIdInstWrapperTemplate = Nan::New<v8::FunctionTemplate>(AnyCoroIdInstWrapper::New);
        anyCoroIdInstWrapperTemplate->SetClassName(Nan::New("AnyCoroIdInst").ToLocalChecked());
        anyCoroIdInstWrapperTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        anyCoroIdInstWrapperTemplate->Inherit(Nan::New(intrinsicTemplate()));

        tmpl.Reset(anyCoroIdInstWrapperTemplate);
    }
}
