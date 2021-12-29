#ifndef LLVM_NODE_ARRAY_REF_H
#define LLVM_NODE_ARRAY_REF_H

#include <nan.h>
#include "llvm/ADT/APInt.h"
#include "../util/from-value-mixin.h"

template <typename T>
class ArrayRefWrapper: public Nan::ObjectWrap, public FromValueMixin<ArrayRefWrapper<T>> {
    public:
        static NAN_MODULE_INIT(Init) {
            auto arrayRef = Nan::GetFunction(Nan::New(ArrayRefTemplate())).ToLocalChecked();
            Nan::Set(target, Nan::New("ArrayRef").ToLocalChecked(), arrayRef);
        }

        static v8::Local<v8::Object> of(llvm::ArrayRef<T> arrayRef) {
            auto constructorFunction = Nan::GetFunction(Nan::New(ArrayRefTemplate())).ToLocalChecked();
            v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(&arrayRef) };
            auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

            Nan::EscapableHandleScope escapeScpoe;
            return escapeScpoe.Escape(instance);
        }
        
        llvm::ArrayRef<T> getArrayRef() {
            return thisArrayRef;
        }

        static bool isInstance(v8::Local<v8::Value> value) {
            return Nan::New(ArrayRefTemplate())->HasInstance(value);
        }

        ArrayRefWrapper(llvm::ArrayRef<T> ref): thisArrayRef(ref) {}
    
    private:
        llvm::ArrayRef<T> thisArrayRef;

        static NAN_METHOD(New) {
            if (!info.IsConstructCall()) {
                return Nan::ThrowTypeError("ArrayRef constructor needs to be called with new");
            }

            if (info.Length() < 1 || !info[0]->IsExternal()) {
                return Nan::ThrowTypeError("Expected type pointer");
            }

            auto *arrayRef = static_cast<llvm::ArrayRef<T>*>(v8::External::Cast(*info[0])->Value());
            auto *wrapper = new ArrayRefWrapper(*arrayRef);
            wrapper->Wrap(info.This());

            info.GetReturnValue().Set(info.This());
        }

        static Nan::Persistent<v8::FunctionTemplate> &ArrayRefTemplate() {
            static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
            if (functionTemplate.IsEmpty()) {
                v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(ArrayRefWrapper::New);
                localTemplate->SetClassName(Nan::New("ArrayRef").ToLocalChecked());
                localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
                functionTemplate.Reset(localTemplate);
            }

            return functionTemplate;
        }
};

#endif //LLVM_NODE_ARRAY_REF_H