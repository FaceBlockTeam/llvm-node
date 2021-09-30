#include "catchswitch-inst.h"
#include "basic-block.h"
#include "nan.h"
#include "value.h"
#include "../util/string.h"
#include <string>

NAN_MODULE_INIT(CatchSwitchInstWrapper::Init) {
    auto catchSwitchInst = Nan::GetFunction(Nan::New(catchSwitchInstTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("CatchSwitchInst").ToLocalChecked(), catchSwitchInst);
}


NAN_METHOD(CatchSwitchInstWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Constructor needs to be called with new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("constructor needs to be called with CatchSwitchInst: external");
    }

    auto* catchSwitchInst = static_cast<llvm::CatchSwitchInst*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new CatchSwitchInstWrapper { catchSwitchInst };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(CatchSwitchInstWrapper::create) {
    if (info.Length() < 3 || !ValueWrapper::isInstance(info[0]) || !BasicBlockWrapper::isInstance(info[1]) || !info[2]->IsUint32()
        || (info.Length() > 3 && !info[3]->IsString())
        || info.Length() > 5) {
            return Nan::ThrowTypeError("create needs to be called with parentPad: Value, unwindDest: BasicBlock, numHandlers: number, nameStr: string");
    }

    auto* catchSwitchInst = CatchSwitchInstWrapper::FromValue(info.Holder())->getCatchSwitchInst();
    auto* parentPad = ValueWrapper::FromValue(info[0])->getValue();
    auto* unwindDest = BasicBlockWrapper::FromValue(info[1])->getBasicBlock();
    uint32_t numHandlers = Nan::To<uint32_t>(info[2]).FromJust();
    auto nameStr = ToString(info[3]);
    catchSwitchInst->Create(parentPad, unwindDest, numHandlers, nameStr);
}

NAN_GETTER(CatchSwitchInstWrapper::getParentPad) {
    auto* catchSwitchInst = CatchSwitchInstWrapper::FromValue(info.Holder())->getCatchSwitchInst();
    auto* value = catchSwitchInst->getParentPad();
    info.GetReturnValue().Set(ValueWrapper::of(value));
}

NAN_SETTER(CatchSwitchInstWrapper::setParentPad) {
    if (!ValueWrapper::isInstance(value)) {
        return Nan::ThrowTypeError("setParentPad needs to be called with parentPad: Value");
    }
    
    auto* parentPad = CatchSwitchInstWrapper::FromValue(value)->getValue();
    auto* catchSwitchInst = CatchSwitchInstWrapper::FromValue(info.Holder())->getCatchSwitchInst();
    catchSwitchInst->setParentPad(parentPad);
}

NAN_GETTER(CatchSwitchInstWrapper::hasUnwindDest) {
    auto* catchSwitchInst = CatchSwitchInstWrapper::FromValue(info.Holder())->getCatchSwitchInst();
    bool hasUnwindDest = catchSwitchInst->hasUnwindDest();
    info.GetReturnValue().Set(hasUnwindDest);
}

NAN_GETTER(CatchSwitchInstWrapper::unwindsToCaller) {
    auto* catchSwitchInst = CatchSwitchInstWrapper::FromValue(info.Holder())->getCatchSwitchInst();
    bool unwindsToCaller = catchSwitchInst->unwindsToCaller();
    info.GetReturnValue().Set(unwindsToCaller);
}

NAN_GETTER(CatchSwitchInstWrapper::getUnwindDest) {
    auto* catchSwitchInst = CatchSwitchInstWrapper::FromValue(info.Holder())->getCatchSwitchInst();
    auto* unwindDest = catchSwitchInst->getUnwindDest();
    info.GetReturnValue().Set(BasicBlockWrapper::of(unwindDest));
}

NAN_SETTER(CatchSwitchInstWrapper::setUnwindDest) {
    if (!BasicBlockWrapper::isInstance(value)) {
        return Nan::ThrowTypeError("setUnwindDest needs to be called with unwindDest: BasicBlock");
    }
    
    auto* unwindDest = BasicBlockWrapper::FromValue(value)->getBasicBlock();
    auto* catchSwitchInst = CatchSwitchInstWrapper::FromValue(info.Holder())->getCatchSwitchInst();
    catchSwitchInst->setUnwindDest(unwindDest);
}

NAN_GETTER(CatchSwitchInstWrapper::getNumHandlers) {
    auto* catchSwitchInst = CatchSwitchInstWrapper::FromValue(info.Holder())->getCatchSwitchInst();
    uint32_t numHandlers = catchSwitchInst->getNumHandlers();
    info.GetReturnValue().Set(numHandlers);
}

NAN_METHOD(CatchSwitchInstWrapper::addHandler) {
    if (info.Length() != 1 || !BasicBlockWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("addHandler needs to be called with dest: BasicBlock");
    }

    auto* dest = BasicBlockWrapper::FromValue(info[0])->getBasicBlock();
    auto* catchSwitchInst = CatchSwitchInstWrapper::FromValue(info.Holder())->getCatchSwitchInst();
    catchSwitchInst->addHandler(dest);    
}

Nan::Persistent<v8::FunctionTemplate>& CatchSwitchInstWrapper::catchSwitchInstTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        auto localTemplate = Nan::New<v8::FunctionTemplate>(CatchSwitchInstWrapper::New);
        localTemplate->SetClassName(Nan::New("CatchSwitchInst").ToLocalChecked());
        localTemplate->Inherit(Nan::New(valueTemplate()));
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);

        Nan::SetMethod(localTemplate, "create", CatchSwitchInstWrapper::create);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("parentPad").ToLocalChecked(), CatchSwitchInstWrapper::getParentPad, CatchSwitchInstWrapper::setParentPad);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("hasUnwindDest").ToLocalChecked(), CatchSwitchInstWrapper::hasUnwindDest);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("unwindsToCaller").ToLocalChecked(), CatchSwitchInstWrapper::unwindsToCaller);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("numHandlers").ToLocalChecked(), CatchSwitchInstWrapper::getNumHandlers);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("unwindDest").ToLocalChecked(), CatchSwitchInstWrapper::getUnwindDest, CatchSwitchInstWrapper::setUnwindDest);
        Nan::SetPrototypeMethod(localTemplate, "addHandler", CatchSwitchInstWrapper::addHandler);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}

llvm::CatchSwitchInst* CatchSwitchInstWrapper::getCatchSwitchInst() {
    return static_cast<llvm::CatchSwitchInst*>(getValue());
}

v8::Local<v8::Object> CatchSwitchInstWrapper::of(llvm::CatchSwitchInst *catchSwitchInst) {
    auto constructorFunction = Nan::GetFunction(Nan::New(catchSwitchInstTemplate())).ToLocalChecked();
    v8::Local<v8::Value> argv[1] = { Nan::New<v8::External>(catchSwitchInst) };
    auto instance = Nan::NewInstance(constructorFunction, 1, argv).ToLocalChecked();

    Nan::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(instance);
}