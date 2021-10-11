#ifndef LLVM_NODE_INTRINSICS_H
#define LLVM_NODE_INTRINSICS_H

#include <nan.h>
#include "../util/from-value-mixin.h"

NAN_METHOD(getName);
NAN_METHOD(getType);
NAN_METHOD(getDeclaration);

#endif //LLVM_NODE_INTRINSICS_H
