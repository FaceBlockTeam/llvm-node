#include <nan.h>
#include <llvm/IR/Instructions.h>
#include "basic-block.h"
#include "constant-int.h"
#include "value.h"
#include "switch-inst.h"

NAN_MODULE_INIT(SwitchInstWrapper::Init) {
    auto switchInst = Nan::GetFunction(Nan::New(switchInstTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("SwitchInst").ToLocalChecked(), switchInst);
}

NAN_METHOD(SwitchInstWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Constructor needs to be called with new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("constructor needs to be called with: SwitchInst: external");
    }

    auto* switchInst = static_cast<llvm::SwitchInst*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new SwitchInstWrapper { switchInst };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(SwitchInstWrapper::create) {
    if (info.Length() < 3 || !ValueWrapper::isInstance(info[0]) || !BasicBlockWrapper::isInstance(info[1]) || !info[2]->IsUint32() ||
        (info.Length() > 3 && !BasicBlockWrapper::isInstance(info[3])) ||
        info.Length() > 4) {
            return Nan::ThrowTypeError("create needs to be called with: value: Value, default: BasicBlock, numCases: number, insertAtEnd?: BasicBlock");
    }

    auto* value = ValueWrapper::FromValue(info[0])->getValue();
    auto* defaultBasicBlock = BasicBlockWrapper::FromValue(info[1])->getBasicBlock();
    uint32_t numCases = Nan::To<uint32_t>(info[2]).FromJust();

    llvm::SwitchInst* switchInst = nullptr;

    if (info.Length() > 3) {
        switchInst = llvm::SwitchInst::Create(value, defaultBasicBlock, numCases);
    } else {
        auto* insertionPoint = BasicBlockWrapper::FromValue(info[3])->getBasicBlock();
        switchInst = llvm::SwitchInst::Create(value, defaultBasicBlock, numCases, insertionPoint);
    }

    info.GetReturnValue().Set(SwitchInstWrapper::of(switchInst));
}

NAN_GETTER(SwitchInstWrapper::getCondition) {
    auto* switchInst = SwitchInstWrapper::FromValue(info.Holder())->getSwitchInst();
    info.GetReturnValue().Set(ValueWrapper::of(switchInst->getCondition()));
}

NAN_SETTER(SwitchInstWrapper::setCondition) {
    if (!ValueWrapper::isInstance(value)) {
        return Nan::ThrowTypeError("setCondition needs to be called with condition: Value");
    }

    auto* switchInst = SwitchInstWrapper::FromValue(info.Holder())->getSwitchInst();
    auto* condition = ValueWrapper::FromValue(value)->getValue();
    switchInst->setCondition(condition);
}

NAN_GETTER(SwitchInstWrapper::getDefaultDest) {
    auto* switchInst = SwitchInstWrapper::FromValue(info.Holder())->getSwitchInst();
    auto* defaultDest = switchInst->getDefaultDest();
    info.GetReturnValue().Set(BasicBlockWrapper::of(defaultDest));
}

NAN_SETTER(SwitchInstWrapper::setDefaultDest) {
    if (!BasicBlockWrapper::isInstance(value)) {
        return Nan::ThrowTypeError("setDefaultDest needs to be called with defaultDest: BasicBlock");
    }

    auto* defaultDest = BasicBlockWrapper::FromValue(value)->getBasicBlock();
    auto* switchInst = SwitchInstWrapper::FromValue(info.Holder())->getSwitchInst();
    switchInst->setDefaultDest(defaultDest);
}

NAN_GETTER(SwitchInstWrapper::getNumCases) {
    auto* switchInst = SwitchInstWrapper::FromValue(info.Holder())->getSwitchInst();
    uint32_t numCases = switchInst->getNumCases();
    info.GetReturnValue().Set(numCases);
}

NAN_METHOD(SwitchInstWrapper::addCase) {
    if (info.Length() < 2 || !ConstantIntWrapper::isInstance(info[0]) || !BasicBlockWrapper::isInstance(info[1]) || info.Length() > 2) {
        return Nan::ThrowTypeError("addCase needs to be called with onVal: ConstantInt, dest: BasicBlock");
    }

    auto* onVal = ConstantIntWrapper::FromValue(info[0])->getConstantInt();
    auto* dest = BasicBlockWrapper::FromValue(info[1])->getBasicBlock();
    auto* switchInst = SwitchInstWrapper::FromValue(info.Holder())->getSwitchInst();
    switchInst->addCase(onVal, dest);
}

NAN_GETTER(SwitchInstWrapper::getNumSuccessors) {
    auto* switchInst = SwitchInstWrapper::FromValue(info.Holder())->getSwitchInst();
    uint32_t numSuccessors = switchInst->getNumSuccessors();
    info.GetReturnValue().Set(numSuccessors);
}

NAN_METHOD(SwitchInstWrapper::getSuccessor) {
    if (info.Length() != 1 || !info[0]->IsUint32()) {
        return Nan::ThrowTypeError("getSuccessor needs to be called with idx: number");
    }

    uint32_t idx = Nan::To<uint32_t>(info[0]).FromJust();
    auto* switchInst = SwitchInstWrapper::FromValue(info.Holder())->getSwitchInst();
    auto* successor = switchInst->getSuccessor(idx);
    info.GetReturnValue().Set(BasicBlockWrapper::of(successor));
}

NAN_METHOD(SwitchInstWrapper::setSuccessor) {
    if (info.Length() != 2 || !info[0]->IsUint32() || !BasicBlockWrapper::isInstance(info[1])) {
        return Nan::ThrowTypeError("getSuccessor needs to be called with idx: number");
    }

    auto* switchInst = SwitchInstWrapper::FromValue(info.Holder())->getSwitchInst();
    uint32_t idx = Nan::To<uint32_t>(info[0]).FromJust();
    auto* newSuccessor = BasicBlockWrapper::FromValue(info[1])->getBasicBlock();
    switchInst->setSuccessor(idx, newSuccessor);
}

Nan::Persistent<v8::FunctionTemplate>& SwitchInstWrapper::switchInstTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        auto localTemplate = Nan::New<v8::FunctionTemplate>(SwitchInstWrapper::New);
        localTemplate->SetClassName(Nan::New("SwitchInst").ToLocalChecked());
        localTemplate->Inherit(Nan::New(valueTemplate()));
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);

        Nan::SetMethod(localTemplate, "create", SwitchInstWrapper::create);
        Nan::SetPrototypeMethod(localTemplate, "addCase", SwitchInstWrapper::addCase);
        Nan::SetPrototypeMethod(localTemplate, "setSuccessor", SwitchInstWrapper::setSuccessor);
        Nan::SetPrototypeMethod(localTemplate, "setSuccessor", SwitchInstWrapper::setSuccessor);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("condition").ToLocalChecked(), SwitchInstWrapper::getCondition, SwitchInstWrapper::setCondition);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("defaultDest").ToLocalChecked(), SwitchInstWrapper::getDefaultDest, SwitchInstWrapper::setDefaultDest);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("numCases").ToLocalChecked(), SwitchInstWrapper::getNumCases);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("numSuccessors").ToLocalChecked(), SwitchInstWrapper::getNumSuccessors);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}

v8::Local<v8::Object> SwitchInstWrapper::of(llvm::SwitchInst *switchInst) {
    auto constructorFunction = Nan::GetFunction(Nan::New(switchInstTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(switchInst) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe {};
    return escapeScpoe.Escape(instance);
}

llvm::SwitchInst* SwitchInstWrapper::getSwitchInst() {
    return static_cast<llvm::SwitchInst*>(getValue());
}