//
// Created by SVK on 17.06.2022.
//
#include "printer.h"
#include "ifs/ExStream.h"
#include "common/common.h"



//ExStringStreamIfs
ExStreamIfs &ExStream::operator<<(const std::string &arg) {
    llvm::outs() <<arg;
    return *this;
}

ExStreamIfs &ExStream::operator<<(const ExColors &arg) {
    llvm::outs() <<arg;
    return *this;
}

void ExStream::finalize() {}






// ExStringStream
ExStreamIfs &ExStringStream::operator<<(const std::string &txt) {
    list_.push_back(txt);
    return *this;
}

ExStreamIfs &ExStringStream::operator<<(const ExColors &arg) {
    return *this;
}

std::string ExStringStream::readAndClear() {
    return std::string();
}

void ExStringStream::finalize() {}


// ExErrorStream
ExStreamIfs &ExErrorStream::operator<<(const std::string &arg) {
    llvm::errs() <<arg;
    return *this;
}

ExStreamIfs &ExErrorStream::operator<<(const ExColors &arg) {
    llvm::errs() <<arg;
    return *this;
}

void ExErrorStream::finalize() {}

