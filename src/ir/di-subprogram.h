#ifndef LLVM_NODE_DI_SUBPROGRAM_H
#define LLVM_NODE_DI_SUBPROGRAM_H
#include <nan.h>

#include <llvm/IR/DebugInfoMetadata.h>
#include "../util/from-value-mixin.h"

class DISubprogramWrapper: public Nan::ObjectWrap, public FromValueMixin<DISubprogramWrapper> {
    public:
        static NAN_MODULE_INIT(Init);
        static v8::Local<v8::Object> of(llvm::DISubprogram *);
        llvm::DISubprogram *getDISubprogram();
        static bool isInstance(v8::Local<v8::Value>);

        DISubprogramWrapper(llvm::DISubprogram *);
    
    private:
        llvm::DISubprogram *diSubprogram;

        static NAN_METHOD(New);
        static Nan::Persistent<v8::FunctionTemplate> &diSubprogramTemplate();
};

#endif //LLVM_NODE_DI_SUBPROGRAM_H