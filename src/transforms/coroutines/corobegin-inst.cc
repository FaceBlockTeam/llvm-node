#include "corobegin-inst.h"
#include "Transforms/Coroutines/CoroInstr.h"

NAN_MODULE_INIT(CoroBeginInstWrapper::Init) {
    auto coroBeginInst = Nan::GetFunction(Nan::New(coroBeginInstTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("CoroBeginInst").ToLocalChecked(), coroBeginInst);
}

v8::Local<v8::Object> CoroBeginInstWrapper::of(llvm::CoroBeginInst* coroBeginInst) {
    auto constructorFunction = Nan::GetFunction(Nan::New(coroBeginInstTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(coroBeginInst)};

    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapableHandleScope {};
    return escapableHandleScope.Escape(instance);
}   

llvm::CoroBeginInst* CoroBeginInstWrapper::getCoroBeginInst() {
    return static_cast<llvm::CoroBeginInst*>(getValue());
}

Nan::Persistent<v8::FunctionTemplate>& CoroBeginInstWrapper::coroBeginInstTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> tmpl {};

    if (tmpl.IsEmpty()) {
        auto coroBeginInstWrapperTemplate = Nan::New<v8::FunctionTemplate>(CoroBeginInstWrapper::New);
        coroBeginInstWrapperTemplate->SetClassName(Nan::New("CoroBeginInst").ToLocalChecked());
        coroBeginInstWrapperTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        coroBeginInstWrapperTemplate->Inherit(Nan::New(intrinsicTemplate()));

        tmpl.Reset(coroBeginInstWrapperTemplate);
    }
}

NAN_METHOD(CoroBeginInstWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *coroBeginInst = static_cast<llvm::CoroBeginInst*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new CoroBeginInstWrapper { coroBeginInst };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}
