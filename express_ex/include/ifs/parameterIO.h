#ifndef PARAMETERIO_H
#define PARAMETERIO_H
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


#include "ifs/parameterIfs.h"


DLL_EXPORT bool readParametersList(std::string database_fname, std::vector<ParameterIfs*>& parameter_list);
DLL_EXPORT std::vector<ParameterIfs*> readParametersList(std::string database_Fame);
DLL_EXPORT ParameterIfs* intersection(ParameterIfs* a, ParameterIfs* b, PrmTypesEn target_ty, const std::string& name);
DLL_EXPORT ParameterIfs* intersection(std::vector<ParameterIfs*> arg_list, PrmTypesEn target_ty,
                                      const std::string& name);
DLL_EXPORT ParameterIfs* retyping(ParameterIfs* a, PrmTypesEn target_ty, const std::string& name);
DLL_EXPORT ParameterIfs* newParameter(std::string name, const std::vector<DataInterval>& interval_list,
                                      bool save_fnames);
DLL_EXPORT ParameterIfs* newParameter(std::string name, const TimeInterval& time_interval,
                                      const std::vector<DataInterval>& interval_list, bool save_fnames = true);

calcMinMaxTy g_calcMinMax_select(PrmTypesEn arg);

class DLL_EXPORT ParametersDB {
   public:


    ParametersDB(const std::vector<std::string>& file_name_list) { addParametersSet(file_name_list); }

    ~ParametersDB() {
        for (auto& i : db_parameters_) delete i;
    }

    bool addParametersSet(const std::vector<std::string>& file_name_list) {
        auto ret = true;
        for (auto file_name : file_name_list) ret &= readParametersList(file_name, db_parameters_);
        return ret;
    }

    ParameterIfs* operator[](std::string name) {
        for (auto& i : db_parameters_)
            if (i->getName() == name) return i;
        return nullptr;
    }

   private:


    std::vector<ParameterIfs*> db_parameters_;

};



#endif