#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "di-node.h"

NAN_MODULE_INIT(DINodeWrapper::Init) {
    auto diNode = Nan::GetFunction(Nan::New(diNodeTemplate())).ToLocalChecked();
    
    auto diFlags = Nan::New<v8::Object>();

    Nan::Set(diFlags, Nan::New("FlagAccessibility").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagAccessibility));
    Nan::Set(diFlags, Nan::New("FlagAllCallsDescribed").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagAllCallsDescribed));
    Nan::Set(diFlags, Nan::New("FlagAppleBlock").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagAppleBlock));
    Nan::Set(diFlags, Nan::New("FlagArtificial").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagArtificial));
    Nan::Set(diFlags, Nan::New("FlagBigEndian").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagBigEndian));
    Nan::Set(diFlags, Nan::New("FlagBitField").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagBitField));
    Nan::Set(diFlags, Nan::New("FlagEnumClass").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagEnumClass));
    Nan::Set(diFlags, Nan::New("FlagExplicit").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagExplicit));
    Nan::Set(diFlags, Nan::New("FlagExportSymbols").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagExportSymbols));
    Nan::Set(diFlags, Nan::New("FlagFwdDecl").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagFwdDecl));
    Nan::Set(diFlags, Nan::New("FlagIndirectVirtualBase").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagIndirectVirtualBase));
    Nan::Set(diFlags, Nan::New("FlagIntroducedVirtual").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagIntroducedVirtual));
    Nan::Set(diFlags, Nan::New("FlagLargest").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagLargest));
    Nan::Set(diFlags, Nan::New("FlagLittleEndian").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagLittleEndian));
    Nan::Set(diFlags, Nan::New("FlagLValueReference").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagLValueReference));
    Nan::Set(diFlags, Nan::New("FlagMultipleInheritance").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagMultipleInheritance));
    Nan::Set(diFlags, Nan::New("FlagNonTrivial").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagNonTrivial));
    Nan::Set(diFlags, Nan::New("FlagNoReturn").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagNoReturn));
    Nan::Set(diFlags, Nan::New("FlagObjcClassComplete").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagObjcClassComplete));
    Nan::Set(diFlags, Nan::New("FlagObjectPointer").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagObjectPointer));
    Nan::Set(diFlags, Nan::New("FlagPrivate").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagPrivate));
    Nan::Set(diFlags, Nan::New("FlagProtected").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagProtected));
    Nan::Set(diFlags, Nan::New("FlagPrototyped").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagPrototyped));
    Nan::Set(diFlags, Nan::New("FlagPtrToMemberRep").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagPtrToMemberRep));
    Nan::Set(diFlags, Nan::New("FlagPublic").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagPublic));
    Nan::Set(diFlags, Nan::New("FlagReservedBit4").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagReservedBit4));
    Nan::Set(diFlags, Nan::New("FlagRValueReference").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagRValueReference));
    Nan::Set(diFlags, Nan::New("FlagSingleInheritance").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagSingleInheritance));
    Nan::Set(diFlags, Nan::New("FlagStaticMember").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagStaticMember));
    Nan::Set(diFlags, Nan::New("FlagThunk").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagThunk));
    Nan::Set(diFlags, Nan::New("FlagTypePassByReference").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagTypePassByReference));
    Nan::Set(diFlags, Nan::New("FlagTypePassByValue").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagTypePassByValue));
    Nan::Set(diFlags, Nan::New("FlagVector").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagVector));
    Nan::Set(diFlags, Nan::New("FlagVirtual").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagVirtual));
    Nan::Set(diFlags, Nan::New("FlagVirtualInheritance").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagVirtualInheritance));
    Nan::Set(diFlags, Nan::New("FlagZero").ToLocalChecked(), Nan::New(llvm::DINode::DIFlags::FlagZero));

    Nan::Set(diNode, Nan::New("DIFlags").ToLocalChecked(), diFlags);
    Nan::Set(target, Nan::New("DINode").ToLocalChecked(), diNode);
}

llvm::DINode *DINodeWrapper::getDINode() {
    return diNode;
}

v8::Local<v8::Object> DINodeWrapper::of(llvm::DINode *diNode) {
    auto constructorFunction = Nan::GetFunction(Nan::New(diNodeTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(diNode) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe;
    return escapeScpoe.Escape(instance);
}

bool DINodeWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(diNodeTemplate())->HasInstance(value);
}

DINodeWrapper::DINodeWrapper(llvm::DINode *node): diNode(node) {}

NAN_METHOD(DINodeWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("DINode constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *diNode = static_cast<llvm::DINode*>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DINodeWrapper(diNode);
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

Nan::Persistent<v8::FunctionTemplate> &DINodeWrapper::diNodeTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(DINodeWrapper::New);
        localTemplate->SetClassName(Nan::New("DINode").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}