#ifndef LLVM_NODE_RESUMEINST_H
#define LLVM_NODE_RESUMEINST_H

#include <nan.h>
#include <llvm/IR/Instructions.h>
#include "value.h"

class ResumeInstWrapper: public ValueWrapper, public FromValueMixin<ResumeInstWrapper> {
public:
    using FromValueMixin<ResumeInstWrapper>::FromValue;

    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::ResumeInst* resumeInst);
    llvm::ResumeInst* getResumeInst();
    

private:
    explicit ResumeInstWrapper(llvm::ResumeInst* resumeInst): ValueWrapper { resumeInst } {
    }

    static NAN_METHOD(New);
    static NAN_METHOD(create);
    
    static Nan::Persistent<v8::FunctionTemplate>& resumeInstTemplate();
};

#endif //LLVM_NODE_RESUMEINST_H