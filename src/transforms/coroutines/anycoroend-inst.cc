#include <nan.h>
#include "anycoroend-inst.h"
#include <Transforms/Coroutines/CoroInstr.h>

NAN_MODULE_INIT(AnyCoroEndInstWrapper::Init) {
    auto anyCoroEndInst = Nan::GetFunction(Nan::New(anyCoroEndInstTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("AnyCoroEndInst").ToLocalChecked(), anyCoroEndInst);
}

NAN_METHOD(AnyCoroEndInstWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *anyCoroEndInst = static_cast<llvm::AnyCoroEndInst*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new AnyCoroEndInstWrapper { anyCoroEndInst };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> AnyCoroEndInstWrapper::of(llvm::AnyCoroEndInst* anyCoroEndInst) {
    auto constructorFunction = Nan::GetFunction(Nan::New(anyCoroEndInstTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(anyCoroEndInst)};

    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapableHandleScope {};
    return escapableHandleScope.Escape(instance);
}

llvm::AnyCoroSuspendInst* AnyCoroEndInstWrapper::getAnyCoroEndInst() {
    return static_cast<llvm::CoroEndInst*>(getValue());
}

Nan::Persistent<v8::FunctionTemplate>& AnyCoroEndInstWrapper::anyCoroEndInstTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> tmpl {};

    if (tmpl.IsEmpty()) {
        auto anyCoroEndInstWrapperTemplate = Nan::New<v8::FunctionTemplate>(AnyCoroEndInstWrapper::New);
        anyCoroEndInstWrapperTemplate->SetClassName(Nan::New("CoroEndInst").ToLocalChecked());
        anyCoroEndInstWrapperTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        anyCoroEndInstWrapperTemplate->Inherit(Nan::New(anyCoroEndInstTemplate()));

        tmpl.Reset(anyCoroEndInstWrapperTemplate);
    }
}