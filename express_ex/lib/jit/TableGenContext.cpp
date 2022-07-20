//
// Created by SVK on 20.07.2022.
//

#include "jit/TableGenContext.h"

#include "jit/table.h"

void TableGenContext::setUint(ExValue_ifs *var) const { table_->setUint(var); }

void TableGenContext::setParameter(ParameterIfs *var) const { table_->parameter_set_.insert(var); }

void TableGenContext::setMaxBufferLength(long long int length) const {

    long long int temp = (long long int)1 << (signed char)(floor(log2(length)) - 1);
    if (table_->max_buffer_length_ > temp) table_->max_buffer_length_ = temp;
}
