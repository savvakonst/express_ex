//
// Created by SVK on 20.07.2022.
//

#ifndef EXPRESS_EX_ENV_TABLEGENCONTEXT_H
#define EXPRESS_EX_ENV_TABLEGENCONTEXT_H

class Table;
class ExValue_ifs;
class ParameterIfs;

class TableGenContext {
   public:
    explicit TableGenContext(Table* arg) { table_ = arg; }
    ~TableGenContext() = default;

    unsigned long long int getUniqueIndex() { return unique_name_counter_++; }
    void setUint(ExValue_ifs* var) const;
    void setParameter(ParameterIfs* var) const;
    void setMaxBufferLength(long long int length) const;

   private:
    Table* table_;
    unsigned long long int unique_name_counter_ = 0;
};

#endif  // EXPRESS_EX_ENV_TABLEGENCONTEXT_H
