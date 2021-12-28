#ifndef LLVM_NODE_INSTRUCTION_H
#define LLVM_NODE_INSTRUCTION_H

#include <nan.h>
#include "llvm/IR/Instruction.h"
#include "../util/from-value-mixin.h"

class InstructionWrapper: public Nan::ObjectWrap, public FromValueMixin<InstructionWrapper> {
    public:
        static NAN_MODULE_INIT(Init);
        llvm::Instruction *getInstruction();
        static v8::Local<v8::Object> of(llvm::Instruction *);
        static bool isInstance(v8::Local<v8::Value>);

        InstructionWrapper(llvm::Instruction *);
    
    private:
        llvm::Instruction *instruction;

        static NAN_METHOD(New);
        static NAN_METHOD(getParent);
        static NAN_METHOD(getFunction);
        static NAN_METHOD(getModule);
        static NAN_METHOD(getNumSuccessors);
        static Nan::Persistent<v8::FunctionTemplate> &instructionTemplate();
};

#endif //LLVM_NODE_INSTRUCTION_H