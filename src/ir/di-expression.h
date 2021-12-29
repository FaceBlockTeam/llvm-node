#ifndef LLVM_NODE_DI_EXPRESSION_H
#define LLVM_NODE_DI_EXPRESSION_H
#include <nan.h>

#include <llvm/IR/DebugInfoMetadata.h>
#include "../util/from-value-mixin.h"

class DIExpressionWrapper: public Nan::ObjectWrap, public FromValueMixin<DIExpressionWrapper> {
    public:
        static NAN_MODULE_INIT(Init);
        static v8::Local<v8::Object> of(llvm::DIExpression *);
        static bool isInstance(v8::Local<v8::Value>);
        llvm::DIExpression *getDIExpression();

        DIExpressionWrapper(llvm::DIExpression *);
    
    private:
        llvm::DIExpression *diExpression;

        static NAN_METHOD(New);

        static Nan::Persistent<v8::FunctionTemplate> &diExpressionTemplate();
};

#endif //LLVM_NODE_DI_EXPRESSION_H