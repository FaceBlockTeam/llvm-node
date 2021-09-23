#include <nan.h>
#include <llvm/IR/Instructions.h>
#include "basic-block.h"
#include "resume-inst.h"
#include "value.h"

NAN_METHOD(ResumeInstWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Constructor needs to be called with new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("constructor needs to be called with: ResumeInst: external");
    }

    auto* resumeInst = static_cast<llvm::ResumeInst*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new ResumeInstWrapper { resumeInst };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(ResumeInstWrapper::create) {
    if (info.Length() != 2 || !ValueWrapper::isInstance(info[0]) || !BasicBlockWrapper::isInstance(info[1])) {
        return Nan::ThrowTypeError("create needs to be called with exn: Value and insertAtEnd: BasicBlock");
    }

    auto* value = ValueWrapper::FromValue(info[0])->getValue();
    auto* basicBlock = BasicBlockWrapper::FromValue(info[1])->getBasicBlock();
    auto* resumeInst = llvm::ResumeInst::Create(value, basicBlock);
    info.GetReturnValue().Set(ResumeInstWrapper::of(resumeInst));
}

v8::Local<v8::Object> ResumeInstWrapper::of(llvm::ResumeInst* resumeInst) {
    auto constructorFunction = Nan::GetFunction(Nan::New(resumeInstTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(resumeInst) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe {};
    return escapeScpoe.Escape(instance);
}

Nan::Persistent<v8::FunctionTemplate>& ResumeInstWrapper::resumeInstTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(ResumeInstWrapper::New);
        localTemplate->SetClassName(Nan::New("ResumeInst").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        localTemplate->Inherit(Nan::New(resumeInstTemplate()));

        Nan::SetPrototypeMethod(localTemplate, "create", ResumeInstWrapper::create);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}

llvm::ResumeInst* ResumeInstWrapper::getResumeInst() {
    return static_cast<llvm::ResumeInst*>(getValue());
}