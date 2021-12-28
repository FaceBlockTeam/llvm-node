#include <nan.h>
#include "llvm/IR/Instruction.h"
#include "instruction.h"
#include "basic-block.h"
#include "module.h"
#include "function.h"

NAN_MODULE_INIT(InstructionWrapper::Init) {
    auto instruction = Nan::GetFunction(Nan::New(instructionTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("DILocation").ToLocalChecked(), instruction);
}

llvm::Instruction *InstructionWrapper::getInstruction() {
    return instruction;
}

v8::Local<v8::Object> InstructionWrapper::of(llvm::Instruction *instruction) {
    auto constructorFunction = Nan::GetFunction(Nan::New(instructionTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(instruction) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe;
    return escapeScpoe.Escape(instance);
}

bool InstructionWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(instructionTemplate())->HasInstance(value);
}

InstructionWrapper::InstructionWrapper(llvm::Instruction *_instruction): instruction(_instruction) {}
    
NAN_METHOD(InstructionWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Instruction constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *instruction = static_cast<llvm::Instruction*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new InstructionWrapper(instruction);
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(InstructionWrapper::getParent) {
    if (info.Length() != 0) {
        return Nan::ThrowSyntaxError("No argument is required.");
    }

    auto *instruction = InstructionWrapper::FromValue(info.Holder())->getInstruction();
    auto *parent = instruction->getParent();
    info.GetReturnValue().Set(BasicBlockWrapper::of(parent));
}

NAN_METHOD(InstructionWrapper::getFunction) {
    if (info.Length() != 0) {
        return Nan::ThrowSyntaxError("No argument is required.");
    }

    auto *instruction = InstructionWrapper::FromValue(info.Holder())->getInstruction();
    auto *function = instruction->getFunction();
    info.GetReturnValue().Set(FunctionWrapper::of(function));
}

NAN_METHOD(InstructionWrapper::getModule) {
    if (info.Length() != 0) {
        return Nan::ThrowSyntaxError("No argument is required.");
    }

    auto *instruction = InstructionWrapper::FromValue(info.Holder())->getInstruction();
    auto *module = instruction->getModule();
    if (module != nullptr) info.GetReturnValue().Set(ModuleWrapper::of(module));
}

NAN_METHOD(InstructionWrapper::getNumSuccessors) {
    if (info.Length() != 0) {
        return Nan::ThrowSyntaxError("No argument is required.");
    }

    auto *instruction = InstructionWrapper::FromValue(info.Holder())->getInstruction();
    unsigned int numSuccessors = instruction->getNumSuccessors();
    info.GetReturnValue().Set(numSuccessors);
}

Nan::Persistent<v8::FunctionTemplate> &InstructionWrapper::instructionTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(InstructionWrapper::New);
        localTemplate->SetClassName(Nan::New("Instruction").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        Nan::SetPrototypeMethod(localTemplate, "getParent", InstructionWrapper::getParent);
        Nan::SetPrototypeMethod(localTemplate, "getModule", InstructionWrapper::getModule);
        Nan::SetPrototypeMethod(localTemplate, "getFunction", InstructionWrapper::getFunction);
        Nan::SetPrototypeMethod(localTemplate, "getNumSuccessors", InstructionWrapper::getNumSuccessors);
        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}