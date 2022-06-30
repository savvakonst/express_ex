#ifndef PARAMETERIO_H
#define PARAMETERIO_H
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ifs/ParameterIfs.h"


DLL_EXPORT bool readParametersList(DatasetsStorage_ifs* storage, const std::string& express_f_name,
                                   std::vector<ParameterIfs*>& parameter_list);

DLL_EXPORT ParameterIfs* intersection(ParameterIfs* a, ParameterIfs* b, PrmTypesEn target_ty, const std::string& name);
DLL_EXPORT ParameterIfs* intersection(std::vector<ParameterIfs*> arg_list, PrmTypesEn target_ty,
                                      const std::string& name);
DLL_EXPORT ParameterIfs* retyping(ParameterIfs* a, PrmTypesEn target_ty, const std::string& name);
DLL_EXPORT ParameterIfs* newParameter(const std::string& name, const std::vector<DataInterval>& interval_list,
                                      bool save_fnames);
DLL_EXPORT ParameterIfs* newParameter(const std::string& name, const TimeInterval& time_interval,
                                      const std::vector<DataInterval>& interval_list, bool save_fnames = true);

calcMinMaxTy gCalcMinMaxSelect(PrmTypesEn arg);



#endif