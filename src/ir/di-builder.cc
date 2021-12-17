#include <nan.h>
#include "di-builder.h"
#include "util/string.h"
#include "util/array.h"

NAN_MODULE_INIT(DIBuilderWrapper::Init) {

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
    if (info.Length() != 1 || !DISubprogamWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("finalizeSubprogram should have received only 1 pointer of DISubporgram");
    }
    auto &diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();
    auto *diSubprogram = DISubprogramWrapper::FromValue(info[0])->getDISubprogram();
    diBuilder.finalizeSubprogram(diSubprogram);
}

NAN_METHOD(DIBuilderWrapper::createCompileUnit) {

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
        || !info[2]->IsNumber() || !info[3]->IsUint32(), !DIFlagsWrapper::isDIFlags(info[4])) {
            return Nan::ThrowTypeError("createInstance shoule be called exactly with 5 arguments");
    }

    auto *derivedType = DITypeWrapper::FromValue(info[0])->getDIType();
    auto *baseType = DITypeWrapper::FromValue(info[1])->getDIType();
    uint64_t baseOffset = Nan::To<uint64_t>(info[2]).FromJust();
    uint32_t vbPtrOffset = Nan::To<uint32_t>(info[3]).FromJust();
    auto flags = DIFlagsWrapper::FromValue(info[4])->getDIFlags();

    auto &diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();

    auto *diDerivedType = diBuilder.createInheritance(derivedType, baseType, baseOffset, vbPtrOffset, flags);

    info.GetReturnValue().Set(DIDerivedType::of(diDerivedType));
}

NAN_METHOD(DIBuilderWrapper::createExpression) {
    if (info.Length() > 1 || (info.Length() == 1 && !info[0]->IsBigUint64Array())) {
        return Nan::ThrowTypeError("createExpression should be called with 2 arguments");
    }

    llvm::DIExpression *diExpression = nullptr;

    auto &diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();

    if (info.Length() == 1) {
        auto addr = toVector<uint64_t>(info[0]);
        diExpression = diBuilder.createExpression(addr);
    } else {
        diExpression = diBuilder.createExpression();
    }

    info.GetReturnValue().Set(DIExpression::of(diExpression));
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
    auto *diSubroutineType = DISubroutineType::FromValue(info[5])->getDISubroutineType();
    uint32_t scopeLine = Nan::To<uint32_t>(info[6]).FromJust();

    auto &diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();

    llvm::DISubprogram *diSubprogram = diBuilder.createFunction(diScope, name, linkageName,
        diFile, lineNo, diSubroutineType, scopeLine);

   info.GetReturnValue().Set(DISubprogram::of(diSubprogram));
}

NAN_METHOD(DIBuilderWrapper::createModule) {

}

Nan::Persistent<v8::Function> &diBuilderConstructor() {
    static Nan::Persistent<v8::Function> constructor {};
    return constructor;
}

