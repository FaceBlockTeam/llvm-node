#include "function.h"
#include "function-type.h"
#include "intrinsics.h"
#include "llvm-context.h"
#include "module.h"
#include "../util/array.h"
#include <llvm/IR/Intrinsics.h>
#include <llvm/IR/Function.h>
#include <nan.h>

NAN_MODULE_INIT(InitIntrinsics) {
    auto intrinsics = Nan::New<v8::Object>();
    Nan::SetMethod(intrinsics, "getName", getName);
    Nan::SetMethod(intrinsics, "getType", getType);
    Nan::SetMethod(intrinsics, "getDeclaration", getDeclaration);

    Nan::Set(target, Nan::New("Intrinsics").ToLocalChecked(), intrinsics);
}

NAN_METHOD(getName) {
    if (info.Length() != 1) {
        return Nan::ThrowTypeError("getName needs to be called with: id: ID");
    }

    uint32_t id = Nan::To<uint32_t>(info[0]).FromJust();

    auto name = llvm::Intrinsic::getName(id).str();

    info.GetReturnValue().Set(Nan::New(name).ToLocalChecked());
}

NAN_METHOD(getType) {
    if (info.Length() < 2 || !ModuleWrapper::isInstance(info[0]) || !info[1]->IsUint32() ||
        (info.Length() == 3 && !(info[2]->IsArray())) ||
        info.Length() > 3) {
        return Nan::ThrowTypeError("getDeclaration needs to be called with: M: Module, id: ID, Tys?: Type[]");
    }

    auto &context = LLVMContextWrapper::FromValue(info[0])->getContext();
    uint32_t id = Nan::To<uint32_t>(info[1]).FromJust();

    llvm::FunctionType *functionType = nullptr;

    if (info.Length() == 3) {
        auto array = info[2].As<v8::Array>();
        std::vector<llvm::Type*> types { array->Length() };

        for (size_t i = 0; i < array->Length(); ++i) {
            auto value = Nan::Get(array, i).ToLocalChecked();

            types[i] = TypeWrapper::FromValue(value)->getType();
        }

        functionType = llvm::Intrinsic::getType(context, id, types);
    } else {
        functionType = llvm::Intrinsic::getType(context, id);
    }

    info.GetReturnValue().Set(FunctionTypeWrapper::of(functionType)); 
}

NAN_METHOD(getDeclaration) {
    if (info.Length() < 2 || !ModuleWrapper::isInstance(info[0]) || !info[1]->IsUint32() ||
        (info.Length() == 3 && !(info[2]->IsArray())) ||
        info.Length() > 3) {
        return Nan::ThrowTypeError("getDeclaration needs to be called with: M: Module, id: ID, Tys?: Type[]");
    }

    auto *module = ModuleWrapper::FromValue(info[0])->getModule();
    uint32_t id = Nan::To<uint32_t>(info[1]).FromJust();

    llvm::Function *function = nullptr;

    if (info.Length() == 3) {
        auto array = info[2].As<v8::Array>();
        std::vector<llvm::Type*> types { array->Length() };

        for (size_t i = 0; i < array->Length(); ++i) {
            auto value = Nan::Get(array, i).ToLocalChecked();

            types[i] = TypeWrapper::FromValue(value)->getType();
        }

        function = llvm::Intrinsic::getDeclaration(module, id, types);
    } else {
        function = llvm::Intrinsic::getDeclaration(module, id);
    }
    info.GetReturnValue().Set(FunctionWrapper::of(function)); 
}
