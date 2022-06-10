#ifndef PARAMETERIO_H
#define PARAMETERIO_H
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "AsyncParameter.h"
#include "ParameterIfs.h"
#include "SyncParameter.h"

#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 5208)
#    pragma warning(disable : 4251)
#    pragma warning(disable : 4100)
#    pragma warning(disable : 4189)
#endif

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
    ParametersDB() {}

    ParametersDB(ParameterIfs* parameterInfo, std::string code) {
        db_parameters_.push_back(parameterInfo);
        calcExtendedInfo(parameterInfo);
    }

    ParametersDB(std::vector<ParameterIfs*>& parameterInfoList, std::string code) {
        db_parameters_ = parameterInfoList;
        for (auto& i : db_parameters_) calcExtendedInfo(i);
    }

    ParametersDB(const std::vector<std::string>& file_name_list) { addParametersSet(file_name_list); }

    ~ParametersDB() {
        for (auto& i : db_parameters_) delete i;
        for (auto& i : output_parameters_) delete i;
    }
    /*
    std::vector<SyncParameter *>&   getOutputParameters() {
        return  readParametersList(std::string database_Fame);
    }*/

    bool addParametersSet(const std::string& file_name) { return readParametersList(file_name, db_parameters_); }

    bool addParametersSet(const std::vector<std::string>& file_name_list) {
        auto ret = true;
        for (auto file_name : file_name_list) ret &= readParametersList(file_name, db_parameters_);
        return ret;
    }

    const std::vector<std::string> getNamesList() {
        std::vector<std::string> namesList;
        for (auto i : db_parameters_) namesList.push_back(i->getName());
        return namesList;
    }

    const std::vector<ParameterIfs*>& getDBParameterList() const { return db_parameters_; }

    ParameterIfs* operator[](std::string name) {
        for (auto& i : db_parameters_)
            if (i->getName() == name) return i;
        return nullptr;
    }

   private:
    bool calcExtendedInfo(ParameterIfs* arg) { return true; }

    /// dbParameters contains all parametersInfo from
    /// all available data sets
    ///
    std::vector<ParameterIfs*> db_parameters_;

    std::vector<ParameterIfs*> output_parameters_;
    std::vector<ParameterIfs*> input_parameters_ifs_;

    std::string error_info_ = "";
};

#ifdef _MSC_VER
#    pragma warning(pop)
#endif

#endif