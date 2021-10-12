#ifndef LLVM_NODE_INTRINSICS_H
#define LLVM_NODE_INTRINSICS_H

#include <nan.h>

NAN_MODULE_INIT(InitIntrinsics);

NAN_METHOD(getName);
NAN_METHOD(getType);
NAN_METHOD(getDeclaration);

#endif //LLVM_NODE_INTRINSICS_H
