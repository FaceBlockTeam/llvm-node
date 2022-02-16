#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include "dwarf.h"

NAN_MODULE_INIT(InitDwarf) {
    auto dwarf = Nan::New<v8::Object>();

    Nan::Set(dwarf, Nan::New("DW_ATE_address").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_address));
    Nan::Set(dwarf, Nan::New("DW_ATE_boolean").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_boolean));
    Nan::Set(dwarf, Nan::New("DW_ATE_complex_float").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_complex_float));
    Nan::Set(dwarf, Nan::New("DW_ATE_float").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_float));
    Nan::Set(dwarf, Nan::New("DW_ATE_signed").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_signed));
    Nan::Set(dwarf, Nan::New("DW_ATE_signed_char").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_signed_char));
    Nan::Set(dwarf, Nan::New("DW_ATE_unsigned").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_unsigned));
    Nan::Set(dwarf, Nan::New("DW_ATE_unsigned_char").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_unsigned_char));

    Nan::Set(target, Nan::New("dwarf").ToLocalChecked(), dwarf);
}