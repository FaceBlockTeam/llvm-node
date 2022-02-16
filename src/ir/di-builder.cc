#include <nan.h>
#include "di-builder.h"
#include "../util/string.h"
#include "../util/array.h"
#include "value.h"
#include "basic-block.h"
#include "instruction.h"
#include "di-node-array.h"
#include "di-type-ref-array.h"
#include "di-location.h"
#include "di-local-variable.h"
#include "di-basic-type.h"
#include "di-composite-type.h"
#include "di-compile-unit.h"
#include "di-file.h"
#include "di-type.h"
#include "di-scope.h"
#include "di-subprogram.h"
#include "di-module.h"
#include "di-expression.h"
#include "di-derived-type.h"
#include "di-flags.h"
#include "di-subroutine-type.h"
#include "module.h"
#include "array-ref.h"
#include "metadata.h"

NAN_MODULE_INIT(DIBuilderWrapper::Init) {
    auto diBuilder = Nan::GetFunction(Nan::New(diBuilderTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("DIBuilder").ToLocalChecked(), diBuilder);
}

NAN_METHOD(DIBuilderWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("DIBuilder constructor needs to be called with new");
    }

    if (info.Length() == 0 || info.Length() > 3 || !ModuleWrapper::isInstance(info[0])
        || (info.Length() == 2 && !info[1]->IsBoolean())
        || (info.Length() == 3 && !DICompileUnitWrapper::isInstance(info[2]))) {
            return Nan::ThrowTypeError("DIBuilder received values of wrong argument type or incorrect number of arguments");
    }

    auto *module = ModuleWrapper::FromValue(info[0])->getModule();
    bool allowUnresolved = true;
    llvm::DICompileUnit *diCompileUnit = nullptr;
    if (info.Length() == 2) allowUnresolved = Nan::To<bool>(info[1]).FromJust();
    if (info.Length() == 3) diCompileUnit = DICompileUnitWrapper::FromValue(info[2])->getDICompileUnit();
    auto *wrapper = new DIBuilderWrapper(*module, allowUnresolved, diCompileUnit);
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

llvm::DIBuilder &DIBuilderWrapper::getDIBuilder() {
    return diBuilder;
}

DIBuilderWrapper::DIBuilderWrapper(llvm::Module &module, bool allowUnresolved = true,
    llvm::DICompileUnit *compileUnit = nullptr): diBuilder(module, allowUnresolved, compileUnit) {
}

NAN_METHOD(DIBuilderWrapper::finalize) {
    if (info.Length() != 0) {
        return Nan::ThrowTypeError("finalize does not receive any arguments");
    }
    auto &diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();
    diBuilder.finalize();
}

NAN_METHOD(DIBuilderWrapper::finalizeSubprogram) {
    if (info.Length() != 1 || !DISubprogramWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("finalizeSubprogram should have received only 1 pointer of DISubporgram");
    }
    auto &diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();
    auto *diSubprogram = DISubprogramWrapper::FromValue(info[0])->getDISubprogram();
    diBuilder.finalizeSubprogram(diSubprogram);
}

NAN_METHOD(DIBuilderWrapper::createCompileUnit) {
    // For now, this function receives only required arguments.
    if (info.Length() != 6 || !info[0]->IsUint32() || !DIFileWrapper::isInstance(info[1])
        || !info[2]->IsString() || !info[3]->IsBoolean() || !info[4]->IsString()
        || !info[5]->IsUint32()) {
            return Nan::ThrowTypeError("createCompileUnit should be called exactly with 6 arguments");
    }

    uint32_t lang = Nan::To<uint32_t>(info[0]).FromJust();
    auto *diFile = DIFileWrapper::FromValue(info[1])->getDIFile();
    std::string producer = ToString(info[2]);
    bool isOptimized = Nan::To<bool>(info[3]).FromJust();
    std::string flags = ToString(info[4]);
    uint32_t runtimeVersion = Nan::To<uint32_t>(info[5]).FromJust();
    auto &diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();

    auto *diCompileUnit = diBuilder.createCompileUnit(lang, diFile, producer, isOptimized, flags, runtimeVersion);

    info.GetReturnValue().Set(DICompileUnitWrapper::of(diCompileUnit));
}

NAN_METHOD(DIBuilderWrapper::createFile) {
    // For now, this function receives only required arguments.
    if (info.Length() != 2) {
        return Nan::ThrowTypeError("createFile receives exactly 2 arguments");
    }

    std::string filename = ToString(info[0]);
    std::string directory = ToString(info[1]);

    auto &diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();
    auto *diFile = diBuilder.createFile(filename, directory);

    info.GetReturnValue().Set(DIFileWrapper::of(diFile));
}

NAN_METHOD(DIBuilderWrapper::createInheritance) {
    if (info.Length() != 5 || !DITypeWrapper::isInstance(info[0]) || !DITypeWrapper::isInstance(info[1])
        || !info[2]->IsNumber() || !info[3]->IsUint32() || !DIFlagsWrapper::isInstance(info[4])) {
            return Nan::ThrowTypeError("createInstance shoule be called exactly with 5 arguments");
    }

    auto *derivedType = DITypeWrapper::FromValue(info[0])->getDIType();
    auto *baseType = DITypeWrapper::FromValue(info[1])->getDIType();
    uint64_t baseOffset = Nan::To<double_t>(info[2]).FromJust();
    uint32_t vbPtrOffset = Nan::To<uint32_t>(info[3]).FromJust();
    auto flags = DIFlagsWrapper::FromValue(info[4])->getDIFlags();

    auto &diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();

    auto *diDerivedType = diBuilder.createInheritance(derivedType, baseType, baseOffset, vbPtrOffset, flags);

    info.GetReturnValue().Set(DIDerivedTypeWrapper::of(diDerivedType));
}

NAN_METHOD(DIBuilderWrapper::createExpression) {
    if (info.Length() > 1 || (info.Length() == 1 && !info[0]->IsBigUint64Array())) {
        return Nan::ThrowTypeError("createExpression should be called with 0 or 1 argument");
    }

    llvm::DIExpression *diExpression = nullptr;

    auto &diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();

    if (info.Length() == 1) {
        auto addr = toVector<uint32_t>(info[0]);
        
        std::vector<uint64_t> newAddr;
        for (auto &element: addr) {
            uint64_t newElement = static_cast<uint64_t>(element);
            newAddr.push_back(newElement);
        }
        diExpression = diBuilder.createExpression(newAddr);
    } else {
        diExpression = diBuilder.createExpression();
    }

    info.GetReturnValue().Set(DIExpressionWrapper::of(diExpression));
}

NAN_METHOD(DIBuilderWrapper::createFunction) {
    // For now, this function receives only required arguments.
    if (info.Length() != 7 || !DIScopeWrapper::isInstance(info[0]) || !info[1]->IsString()
        || !info[2]->IsString() || !DIFileWrapper::isInstance(info[3]) || !info[4]->IsUint32()
        || !DISubroutineTypeWrapper::isInstance(info[5]) || !info[6]->IsUint32()) {
            return Nan::ThrowTypeError("createFunction shoule receive only 7 arguments");
    }

    auto *diScope = DIScopeWrapper::FromValue(info[0])->getDIScope();
    std::string name = ToString(info[1]);
    std::string linkageName = ToString(info[2]);
    auto *diFile = DIFileWrapper::FromValue(info[3])->getDIFile();
    uint32_t lineNo = Nan::To<uint32_t>(info[4]).FromJust();
    auto *diSubroutineType = DISubroutineTypeWrapper::FromValue(info[5])->getDISubroutineType();
    uint32_t scopeLine = Nan::To<uint32_t>(info[6]).FromJust();

    auto &diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();

    llvm::DISubprogram *diSubprogram = diBuilder.createFunction(diScope, name, linkageName,
        diFile, lineNo, diSubroutineType, scopeLine);

   info.GetReturnValue().Set(DISubprogramWrapper::of(diSubprogram));
}

NAN_METHOD(DIBuilderWrapper::createModule) {
    // For now, this function receives only required arguments.
    if (info.Length() != 4 || !DIScopeWrapper::isInstance(info[0]) || !info[1]->IsString()
        || !info[2]->IsString() || !info[3]->IsString()) {
            return Nan::ThrowTypeError("createModule should be called only with 4 arguments");
    }

    auto *diScope = DIScopeWrapper::FromValue(info[0])->getDIScope();
    std::string name = ToString(info[1]);
    std::string configMacros = ToString(info[2]);
    std::string includePath = ToString(info[3]);

    auto &diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();
    auto *diModule = diBuilder.createModule(diScope, name, configMacros, includePath);

    info.GetReturnValue().Set(DIModuleWrapper::of(diModule));
}


NAN_METHOD(DIBuilderWrapper::insertDeclare) {
    if (info.Length() != 5 || !ValueWrapper::isInstance(info[0])
        || !DILocalVariableWrapper::isInstance(info[1]) || !DIExpressionWrapper::isInstance(info[2])
        || !DILocationWrapper::isInstance(info[3]) || !BasicBlockWrapper::isInstance(info[4])) {
            return Nan::ThrowSyntaxError("insertDeclare should have received 5 arguments of correct type");
    }

    auto &diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();
    auto *value = ValueWrapper::FromValue(info[0])->getValue();
    auto *diLocalVariable = DILocalVariableWrapper::FromValue(info[1])->getDILocalVariable();
    auto *diExpression = DIExpressionWrapper::FromValue(info[2])->getDIExpression();
    auto *diLocation = DILocationWrapper::FromValue(info[3])->getDILocation();
    auto *basicBlock = BasicBlockWrapper::FromValue(info[4])->getBasicBlock();
    diBuilder.insertDeclare(value, diLocalVariable, diExpression, diLocation, basicBlock);
}

NAN_METHOD(DIBuilderWrapper::insertDbgValueIntrinsic) {
    if (info.Length() != 5 || !ValueWrapper::isInstance(info[0])
        || !DILocalVariableWrapper::isInstance(info[1]) || !DIExpressionWrapper::isInstance(info[2])
        || !DILocationWrapper::isInstance(info[3]) || !BasicBlockWrapper::isInstance(info[4])) {
            return Nan::ThrowSyntaxError("insertDbgValueIntrinsic should have received 5 arguments of correct type");
    }

    auto &diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();
    auto *value = ValueWrapper::FromValue(info[0])->getValue();
    auto *diLocalVariable = DILocalVariableWrapper::FromValue(info[1])->getDILocalVariable();
    auto *diExpression = DIExpressionWrapper::FromValue(info[2])->getDIExpression();
    auto *diLocation = DILocationWrapper::FromValue(info[3])->getDILocation();
    auto *basicBlock = BasicBlockWrapper::FromValue(info[4])->getBasicBlock();
    diBuilder.insertDbgValueIntrinsic(value, diLocalVariable, diExpression, diLocation, basicBlock);
}

NAN_METHOD(DIBuilderWrapper::getOrCreateTypeArray) {
    if (info.Length() != 1 || !info[0]->IsArray()) {
        return Nan::ThrowSyntaxError("getOrCreateTypeArray should have received 1 argument of correct type");
    }

    auto &diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();
    auto arr = info[0].As<v8::Array>();
    uint32_t len = arr->Length();
    std::vector<llvm::Metadata*> metadataArray(len);

    for (uint32_t i = 0; i < len; ++i) {
        auto localValue = Nan::Get(arr, i).ToLocalChecked();
        metadataArray[i] = MetadataWrapper::FromValue(localValue)->getMetadata();
    }
    auto diTypeRefArray = diBuilder.getOrCreateTypeArray(metadataArray);
    info.GetReturnValue().Set(DITypeRefArrayWrapper::of(diTypeRefArray));
}

NAN_METHOD(DIBuilderWrapper::getOrCreateArray) {
    if (info.Length() != 1 || !info[0]->IsArray()) {
        return Nan::ThrowSyntaxError("getOrCreateArray should have received 1 argument of correct type");
    }

    auto &diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();
    auto arr = info[0].As<v8::Array>();
    uint32_t len = arr->Length();
    std::vector<llvm::Metadata*> metadataArray(len);

    for (uint32_t i = 0; i < len; ++i) {
        auto localValue = Nan::Get(arr, i).ToLocalChecked();
        metadataArray[i] = MetadataWrapper::FromValue(localValue)->getMetadata();
    }
    auto diNodeArray = diBuilder.getOrCreateArray(metadataArray);
    info.GetReturnValue().Set(DINodeArrayWrapper::of(diNodeArray));
}

NAN_METHOD(DIBuilderWrapper::createBasicType) {
    if (info.Length() != 3 || !info[0]->IsString() || !info[1]->IsUint32() || !info[2]->IsUint32()) {
        return Nan::ThrowSyntaxError("createBasicType should have received 3 arguments of correct type");
    }

    auto &diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();
    std::string name = ToString(info[0]);
    uint64_t sizeInBits = Nan::To<uint32_t>(info[1]).FromJust();
    uint32_t encoding = Nan::To<uint32_t>(info[2]).FromJust();
    auto *diBasicType = diBuilder.createBasicType(name, sizeInBits, encoding);
    info.GetReturnValue().Set(DIBasicTypeWrapper::of(diBasicType));
}

NAN_METHOD(DIBuilderWrapper::createAutoVariable) {
    if (info.Length() != 5 || !DIScopeWrapper::isInstance(info[0]) || !info[1]->IsString() || !DIFileWrapper::isInstance(info[2])
        || !info[3]->IsUint32() || !DITypeWrapper::isInstance(info[4])) {
            return Nan::ThrowSyntaxError("createAutoVariable should have received 5 arguments of correct type");
    }

    auto &diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();
    auto *diScope = DIScopeWrapper::FromValue(info[0])->getDIScope();
    std::string name = ToString(info[1]);
    auto *diFile = DIFileWrapper::FromValue(info[2])->getDIFile();
    uint32_t line = Nan::To<uint32_t>(info[3]).FromJust();
    auto *diType = DITypeWrapper::FromValue(info[4])->getDIType();
    auto *diLocalVariable = diBuilder.createAutoVariable(diScope, name, diFile, line, diType);

    info.GetReturnValue().Set(DILocalVariableWrapper::of(diLocalVariable));
}

NAN_METHOD(DIBuilderWrapper::createParameterVariable) {
    if (info.Length() != 6 || !DIScopeWrapper::isInstance(info[0]) || !info[1]->IsString() || !info[2]->IsUint32()
        || !DIFileWrapper::isInstance(info[3]) || !info[4]->IsUint32() || !DITypeWrapper::isInstance(info[5])) {
            return Nan::ThrowSyntaxError("createParameterVariable should have received 6 arguments of correct type");
    }

    auto &diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();
    auto *diScope = DIScopeWrapper::FromValue(info[0])->getDIScope();
    std::string name = ToString(info[1]);
    uint32_t argNo = Nan::To<uint32_t>(info[2]).FromJust();
    auto *diFile = DIFileWrapper::FromValue(info[3])->getDIFile();
    uint32_t line = Nan::To<uint32_t>(info[4]).FromJust();
    auto *diType = DITypeWrapper::FromValue(info[5])->getDIType();
    auto *diLocalVariable = diBuilder.createParameterVariable(diScope, name, argNo, diFile, line, diType);

    info.GetReturnValue().Set(DILocalVariableWrapper::of(diLocalVariable));
}

NAN_METHOD(DIBuilderWrapper::createSubroutineType) {
    if (info.Length() != 1 || !DITypeRefArrayWrapper::isInstance(info[0])) {
        return Nan::ThrowSyntaxError("createSubroutineType should have received 1 argument of correct type");
    }
    
    auto &diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();
    auto typeRefArray = DITypeRefArrayWrapper::FromValue(info[0])->getDITypeRefArray();
    auto *subroutineType = diBuilder.createSubroutineType(typeRefArray);
    info.GetReturnValue().Set(DISubroutineTypeWrapper::of(subroutineType));
}

NAN_METHOD(DIBuilderWrapper::createStructType) {
    if (info.Length() != 9 || !DIScopeWrapper::isInstance(info[0]) || !info[1]->IsString() || !DIFileWrapper::isInstance(info[2])
        || !info[3]->IsUint32() || !info[4]->IsUint32() || !info[5]->IsUint32()
        || !DIFlagsWrapper::isInstance(info[6]) || !DITypeWrapper::isInstance(info[7]) || !DINodeArrayWrapper::isInstance(info[8])) {
            return Nan::ThrowSyntaxError("createStructType should have received 0 arguments");
    }

    auto &diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();
    auto *diScope = DIScopeWrapper::FromValue(info[0])->getDIScope();
    std::string name = ToString(info[1]);
    auto *diFile = DIFileWrapper::FromValue(info[2])->getDIFile();
    uint32_t line = Nan::To<uint32_t>(info[3]).FromJust();
    uint32_t size = Nan::To<uint32_t>(info[4]).FromJust();
    uint32_t align = Nan::To<uint32_t>(info[5]).FromJust();
    auto diFlags = DIFlagsWrapper::FromValue(info[6])->getDIFlags();
    auto *diType = DITypeWrapper::FromValue(info[7])->getDIType();
    auto diNodeArray = DINodeArrayWrapper::FromValue(info[8])->getDINodeArray();

    auto *diCompositeType = diBuilder.createStructType(diScope, name, diFile, line, size, align, diFlags, diType, diNodeArray);
    info.GetReturnValue().Set(DICompositeTypeWrapper::of(diCompositeType));
}

NAN_METHOD(DIBuilderWrapper::createUnspecifiedType) {
    if (info.Length() != 1 || !info[0]->IsString()) {
        return Nan::ThrowSyntaxError("createUnspecifiedType should have received 1 argument");
    }

    auto &diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();
    std::string name = ToString(info[0]);

    auto *diBasicType = diBuilder.createUnspecifiedType(name);
    info.GetReturnValue().Set(DIBasicTypeWrapper::of(diBasicType));
}

Nan::Persistent<v8::FunctionTemplate> &DIBuilderWrapper::diBuilderTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(DIBuilderWrapper::New);
        localTemplate->SetClassName(Nan::New("DIBuilder").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        Nan::SetPrototypeMethod(localTemplate, "finalize", DIBuilderWrapper::finalize);
        Nan::SetPrototypeMethod(localTemplate, "finalizeSubprogram", DIBuilderWrapper::finalizeSubprogram);
        Nan::SetPrototypeMethod(localTemplate, "createCompileUnit", DIBuilderWrapper::createCompileUnit);
        Nan::SetPrototypeMethod(localTemplate, "createFile", DIBuilderWrapper::createFile);
        Nan::SetPrototypeMethod(localTemplate, "createInheritance", DIBuilderWrapper::createInheritance);
        Nan::SetPrototypeMethod(localTemplate, "createExpression", DIBuilderWrapper::createExpression);
        Nan::SetPrototypeMethod(localTemplate, "createFunction", DIBuilderWrapper::createFunction);
        Nan::SetPrototypeMethod(localTemplate, "createModule", DIBuilderWrapper::createModule);
        Nan::SetPrototypeMethod(localTemplate, "insertDeclare", DIBuilderWrapper::insertDeclare);
        Nan::SetPrototypeMethod(localTemplate, "insertDbgValueIntrinsic", DIBuilderWrapper::insertDbgValueIntrinsic);
        Nan::SetPrototypeMethod(localTemplate, "getOrCreateTypeArray", DIBuilderWrapper::getOrCreateTypeArray);
        Nan::SetPrototypeMethod(localTemplate, "getOrCreateArray", DIBuilderWrapper::getOrCreateArray);
        Nan::SetPrototypeMethod(localTemplate, "createBasicType", DIBuilderWrapper::createBasicType);
        Nan::SetPrototypeMethod(localTemplate, "createAutoVariable", DIBuilderWrapper::createAutoVariable);
        Nan::SetPrototypeMethod(localTemplate, "createParameterVariable", DIBuilderWrapper::createParameterVariable);
        Nan::SetPrototypeMethod(localTemplate, "createSubroutineType", DIBuilderWrapper::createSubroutineType);
        Nan::SetPrototypeMethod(localTemplate, "createStructType", DIBuilderWrapper::createStructType);
        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}