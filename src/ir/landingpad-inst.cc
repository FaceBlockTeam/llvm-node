#include "constant.h"
#include "landingpad-inst.h"
#include "nan.h"
#include "type.h"

NAN_MODULE_INIT(LandingPadInstWrapper::Init) {
    auto landingPadInst = Nan::GetFunction(Nan::New(landingPadInstTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("LandingPadInst").ToLocalChecked(), landingPadInst);
}

NAN_METHOD(LandingPadInstWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("The constructor of LandingPadInst needs to be called with new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("The constructor of LandingPadInst needs to be called with LandingPadInst: External");
    }

    auto* value = static_cast<llvm::LandingPadInst*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new LandingPadInstWrapper { value };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_GETTER(LandingPadInstWrapper::getNumClauses) {
    auto* wrapper = LandingPadInstWrapper::FromValue(info.Holder());
    auto numClauses = wrapper->getLandingPadInst()->getNumClauses();
    info.GetReturnValue().Set(numClauses);
}

NAN_METHOD(LandingPadInstWrapper::addClause) {
    if (info.Length() != 1 || !ValueWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("addClause needs Constant");
    }

    auto* clause = ConstantWrapper::FromValue(info[0])->getConstant();
    auto* wrapper = LandingPadInstWrapper::FromValue(info.Holder());
    wrapper->getLandingPadInst()->addClause(clause);
}

NAN_METHOD(LandingPadInstWrapper::getClause) {
    if (!info[0]->IsUint32()) {
        return Nan::ThrowTypeError("getClause needs to be called with an integer greater than or equal to 0");
    }
    auto idx = Nan::To<uint32_t>(info[0]).FromJust();
    auto* wrapper = LandingPadInstWrapper::FromValue(info.Holder());
    llvm::Constant* constant = wrapper->getLandingPadInst()->getClause(idx);
    info.GetReturnValue().Set(ConstantWrapper::of(constant));
}

NAN_SETTER(LandingPadInstWrapper::setCleanup) {
    if (!value->IsBoolean()) {
        return Nan::ThrowTypeError("setCleanup needs to be called with a boolean");
    }

    bool setCleanup = Nan::To<bool>(value).FromJust();
    auto* wrapper = LandingPadInstWrapper::FromValue(info.Holder());
    wrapper->getLandingPadInst()->setCleanup(setCleanup);
}

NAN_GETTER(LandingPadInstWrapper::isCleanup) {
    auto* wrapper = LandingPadInstWrapper::FromValue(info.Holder());
    bool isCleanup = wrapper->getLandingPadInst()->isCleanup();
    info.GetReturnValue().Set(isCleanup);
}

NAN_METHOD(LandingPadInstWrapper::isCatch) {
    if (!info[0]->IsUint32()) {
        return Nan::ThrowTypeError("isCatch needs to be called with an integer greater than or equal to 0");
    }

    auto idx = Nan::To<uint32_t>(info[0]).FromJust();
    auto* wrapper = LandingPadInstWrapper::FromValue(info.Holder());
    bool isCatch = wrapper->getLandingPadInst()->isCatch(idx);
    info.GetReturnValue().Set(isCatch);
}

NAN_METHOD(LandingPadInstWrapper::isFilter) {
    if (!info[0]->IsUint32()) {
        return Nan::ThrowTypeError("isFilter needs to be called with an integer greater than or equal to 0");
    }

    auto idx = Nan::To<uint32_t>(info[0]).FromJust();
    auto* wrapper = LandingPadInstWrapper::FromValue(info.Holder());
    bool isFilter = wrapper->getLandingPadInst()->isFilter(idx);

    info.GetReturnValue().Set(isFilter);
}

Nan::Persistent<v8::FunctionTemplate>& LandingPadInstWrapper::landingPadInstTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(LandingPadInstWrapper::New);
        localTemplate->SetClassName(Nan::New("LandingPadInst").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        localTemplate->Inherit(Nan::New(landingPadInstTemplate()));

        Nan::SetMethod(localTemplate, "isCatch", LandingPadInstWrapper::isCatch);
        Nan::SetMethod(localTemplate, "isFilter", LandingPadInstWrapper::isFilter);
        Nan::SetMethod(localTemplate, "getClause", LandingPadInstWrapper::getClause);
        Nan::SetMethod(localTemplate, "addClause", LandingPadInstWrapper::addClause);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("isCleanup").ToLocalChecked(), LandingPadInstWrapper::isCleanup, LandingPadInstWrapper::setCleanup);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("numClauses").ToLocalChecked(), LandingPadInstWrapper::getNumClauses);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}

llvm::LandingPadInst* LandingPadInstWrapper::getLandingPadInst() {
    return static_cast<llvm::LandingPadInst*>(getValue());
}