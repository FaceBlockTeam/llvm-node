#include <nan.h>
#include "coroend-inst.h"
#include <Transforms/Coroutines/CoroInstr.h>

NAN_MODULE_INIT(CoroEndInstWrapper::Init) {
    auto coroEndInst = Nan::GetFunction(Nan::New(coroEndInstTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("CoroEndInst").ToLocalChecked(), coroEndInst);
}

NAN_METHOD(CoroEndInstWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *coroEndInst = static_cast<llvm::CoroEndInst*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new CoroEndInstWrapper { coroEndInst };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> CoroEndInstWrapper::of(llvm::CoroEndInst* coroEndInst) {
    auto constructorFunction = Nan::GetFunction(Nan::New(coroEndInstTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(coroEndInst)};

    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapableHandleScope {};
    return escapableHandleScope.Escape(instance);
}

llvm::CoroSuspendInst* CoroEndInstWrapper::getCoroEndInst() {
    return static_cast<llvm::CoroEndInst*>(getValue());
}

Nan::Persistent<v8::FunctionTemplate>& CoroEndInstWrapper::coroEndInstTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> tmpl {};

    if (tmpl.IsEmpty()) {
        auto coroEndInstWrapperTemplate = Nan::New<v8::FunctionTemplate>(CoroEndInstWrapper::New);
        coroEndInstWrapperTemplate->SetClassName(Nan::New("CoroEndInst").ToLocalChecked());
        coroEndInstWrapperTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        coroEndInstWrapperTemplate->Inherit(Nan::New(coroEndInstTemplate()));

        tmpl.Reset(coroEndInstWrapperTemplate);
    }
}