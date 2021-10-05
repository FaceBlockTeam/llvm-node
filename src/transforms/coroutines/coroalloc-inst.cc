#include <nan.h>
#include "Transforms/Coroutines/CoroInstr.h"
#include "coroalloc-inst.h"

NAN_MODULE_INIT(CoroAllocInstWrapper::Init) {
    auto coroAllocInst = Nan::GetFunction(Nan::New(coroAllocInstTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("CoroAllocInst").ToLocalChecked(), coroAllocInst);
}

v8::Local<v8::Object> CoroAllocInstWrapper::of(llvm::CoroAllocInst* coroAllocInst) {
    auto constructorFunction = Nan::GetFunction(Nan::New(coroAllocInstTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(coroAllocInst)};

    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapableHandleScope {};
    return escapableHandleScope.Escape(instance);
}

llvm::CoroAllocInst* CoroAllocInstWrapper::getCoroAllocInst() {
    return static_cast<llvm::CoroAllocInst*>(getValue());
}

NAN_METHOD(CoroAllocInstWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *coroAllocInst = static_cast<llvm::CoroAllocInst*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new CoroAllocInstWrapper { coroAllocInst };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(CoroAllocInstWrapper::getCoroSave) {
    auto *coroAllocInst = CoroAllocInstWrapper::FromValue(info.Holder())->getCoroAllocInst();
    auto *coroCoroSave = coroAllocInst->getCoroSave();
    info.GetReturnValue().Set(CoroAllocInstWrapper::of(coroAllocInst));
}

Nan::Persistent<v8::FunctionTemplate>& CoroAllocInstWrapper::coroAllocInstTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> tmpl {};

    if (tmpl.IsEmpty()) {
        auto coroAllocInstWrapperTemplate = Nan::New<v8::FunctionTemplate>(AnyCoroIdInstWrapper::New);
        coroAllocInstWrapperTemplate->SetClassName(Nan::New("AnyCoroIdInst").ToLocalChecked());
        coroAllocInstWrapperTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        coroAllocInstWrapperTemplate->Inherit(Nan::New(intrinsicTemplate()));

        tmpl.Reset(coroAllocInstWrapperTemplate);
    }
}
