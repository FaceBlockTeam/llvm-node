#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "di-variable.h"

NAN_MODULE_INIT(DIVariableWrapper::Init) {

}

llvm::DIVariable *DIVariableWrapper::getDIVariable() {
    return diVariable;
}

v8::Local<v8::Object> DIVariableWrapper::of(llvm::DIVariable * diVariable) {

}

bool DIVariableWrapper::isInstance(v8::Local<v8::Value> value) {

}

DIVariableWrapper::DIVariableWrapper(llvm::DIVariable *variable): diVariable(variable) {}
    
NAN_METHOD(DIVariableWrapper::New) {

}

NAN_METHOD(DIVariableWrapper::getLine) {

}

NAN_METHOD(DIVariableWrapper::getScope) {

}

NAN_METHOD(DIVariableWrapper::getName) {

}

NAN_METHOD(DIVariableWrapper::getFile) {

}

NAN_METHOD(DIVariableWrapper::getType) {

}

NAN_METHOD(DIVariableWrapper::getFilename) {

}

NAN_METHOD(DIVariableWrapper::getDirectory) {

}

NAN_METHOD(DIVariableWrapper::getSizeInBits) {

}

Nan::Persistent<v8::FunctionTemplate> &DIVariableWrapper::diVariableTemplate() {

}