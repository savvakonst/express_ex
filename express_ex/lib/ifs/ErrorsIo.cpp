//
// Created by SVK on 17.06.2022.
//
#include "ifs/printer.h"
#include "ifs/ExStream.h"
#include "common/common.h"



//ExStringStreamIfs
ExStringStreamIfs &ExStream::operator<<(const std::string &arg) {
    llvm::outs() <<arg;
    return *this;
}

ExStringStreamIfs &ExStream::operator<<(const ExColors &arg) {
    llvm::outs() <<arg;
    return *this;
}



// ExStringStream
ExStringStreamIfs &ExStringStream::operator<<(const std::string &txt) {
    list_.push_back(txt);
    return *this;
}

ExStringStreamIfs &ExStringStream::operator<<(const ExColors &arg) {
    return *this;
}

std::string ExStringStream::readAndClear() {
    return std::string();
}


// ExErrorStream
ExStringStreamIfs &ExErrorStream::operator<<(const std::string &arg) {
    llvm::errs() <<arg;
    return *this;
}

ExStringStreamIfs &ExErrorStream::operator<<(const ExColors &arg) {
    llvm::errs() <<arg;
    return *this;
}

