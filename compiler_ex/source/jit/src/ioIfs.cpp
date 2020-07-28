#include <iostream>
#include <strstream>
#include <fstream>
#include "ioIfs.h"
#include "llvm/Support/JSON.h"
using namespace llvm;

TypeEn RPMType2JITType(RPMTypesEn arg) {

    switch (arg)
    {
    case RPMTypesEn::PRM_TYPE_U08:return TypeEn::unknown_jty;
    case RPMTypesEn::PRM_TYPE_U16:return TypeEn::unknown_jty;
    case RPMTypesEn::PRM_TYPE_U32:return TypeEn::unknown_jty;
    case RPMTypesEn::PRM_TYPE_U64:return TypeEn::unknown_jty;
    case RPMTypesEn::PRM_TYPE_I08:return TypeEn::int8_jty;
    case RPMTypesEn::PRM_TYPE_I16:return TypeEn::int16_jty;
    case RPMTypesEn::PRM_TYPE_I32:return TypeEn::int32_jty;
    case RPMTypesEn::PRM_TYPE_I64:return TypeEn::int64_jty;
    case RPMTypesEn::PRM_TYPE_F32:return TypeEn::float_jty;
    case RPMTypesEn::PRM_TYPE_F64:return TypeEn::double_jty;
    case RPMTypesEn::PRM_TYPE_U08_T:return TypeEn::unknown_jty;
    case RPMTypesEn::PRM_TYPE_U16_T:return TypeEn::unknown_jty;
    case RPMTypesEn::PRM_TYPE_U32_T:return TypeEn::unknown_jty;
    case RPMTypesEn::PRM_TYPE_U64_T:return TypeEn::unknown_jty;
    case RPMTypesEn::PRM_TYPE_I08_T:return TypeEn::int8_jty;
    case RPMTypesEn::PRM_TYPE_I16_T:return TypeEn::int16_jty;
    case RPMTypesEn::PRM_TYPE_I32_T:return TypeEn::int32_jty;
    case RPMTypesEn::PRM_TYPE_I64_T:return TypeEn::int64_jty;
    case RPMTypesEn::PRM_TYPE_F32_T:return TypeEn::float_jty;
    case RPMTypesEn::PRM_TYPE_F64_T:return TypeEn::double_jty;
    default:return TypeEn::unknown_jty;
    }
}

std::string toString(RPMTypesEn arg) {
#define CASE_OP(NAME) case RPMTypesEn::NAME:return #NAME;
    switch (arg)
    {
        CASE_OP(PRM_TYPE_U08);
        CASE_OP(PRM_TYPE_U16);
        CASE_OP(PRM_TYPE_U32);
        CASE_OP(PRM_TYPE_U64);
        CASE_OP(PRM_TYPE_I08);
        CASE_OP(PRM_TYPE_I16);
        CASE_OP(PRM_TYPE_I32);
        CASE_OP(PRM_TYPE_I64);
        CASE_OP(PRM_TYPE_F32);
        CASE_OP(PRM_TYPE_F64);
        CASE_OP(PRM_TYPE_U08_T);
        CASE_OP(PRM_TYPE_U16_T);
        CASE_OP(PRM_TYPE_U32_T);
        CASE_OP(PRM_TYPE_U64_T);
        CASE_OP(PRM_TYPE_I08_T);
        CASE_OP(PRM_TYPE_I16_T);
        CASE_OP(PRM_TYPE_I32_T);
        CASE_OP(PRM_TYPE_I64_T);
        CASE_OP(PRM_TYPE_F32_T);
        CASE_OP(PRM_TYPE_F64_T);
    default:return "TypeEn::unknown_jty";
    }
#undef CASE_OP
}



llvm::raw_ostream &stream(llvm::raw_ostream &OS,const ParameterInfo & di, std::string offset) {
    OS << offset << "ParameterInfo{\n";
    OS << offset << "  parameterName: " << di.parameterName << "\n";
    OS << offset << "  intervalList: [" << "\n";
    for (auto interval : di.intervalList)
        stream(OS,interval,"    ");
    OS << offset << "  ]\n";
    if (di.extendedInfo){
        OS << offset << "  extendedInfo: "  << "\n";
        stream(OS, *(di.extendedInfo), "    ");
    }
    else
        OS << offset << "  extendedInfo: " << di.extendedInfo << "\n";
    OS << offset << "}\n";
    return OS;
}

llvm::raw_ostream &stream(llvm::raw_ostream &OS, const DataInterval & di,std::string offset) {
    OS << offset << "DataInterval{\n";
    OS << offset << "  type: " << di.type << "\n";
    OS << offset << "  offs: " << di.offs << "\n";
    OS << offset << "  size: " << di.size << "\n";
    OS << offset << "  frequency: " << di.frequency << "\n";
    OS << offset << "  timeInterval.end: " << di.timeInterval.end << "\n";
    OS << offset << "  timeInterval.bgn: " << di.timeInterval.bgn << "\n";
    OS << offset << "  fileName: " << di.fileName << "\n";
    OS << offset << "}\n";
    return OS;
}

llvm::raw_ostream &stream(llvm::raw_ostream &OS, const ExtendedInfo & di, std::string offset) {
    OS << offset << "ExtendedInfo{\n";
    OS << offset << "  virtualSize: " << di.virtualSize << "\n";
    OS << offset << "  frequency: " << di.frequency << "\n";
    OS << offset << "  timeInterval.end: " << di.timeInterval.end << "\n";
    OS << offset << "  timeInterval.bgn: " << di.timeInterval.bgn << "\n";
    OS << offset << "  jitType: " << toString(di.jitType) << "\n";
    OS << offset << "}\n";
    return OS;
}


bool fromJSON(const json::Value &DataFragment, DataInterval & dataInterval) {

    json::ObjectMapper O(DataFragment);
    bool ret= true;

    ret&=O.map("Frequency", dataInterval.frequency);
    ret&=O.map("Offset", dataInterval.offs);
    ret&=O.map("Size", dataInterval.size);
    ret&=O.map("Type", dataInterval.intRepresintationType);

    ret&=O.map("sTime.Begin", dataInterval.timeInterval.bgn);
    ret&=O.map("sTime.End", dataInterval.timeInterval.end);

    return ret;
}


DataInterval getDataInterval(json::Value &DataFragment, json::Array &DataFilesList) {

    DataInterval dataInterval;
    fromJSON(DataFragment, dataInterval);

    int64_t dataIndex;
    json::ObjectMapper O(DataFragment);
    O.map("Index", dataIndex);


    for (auto i : DataFilesList) {
        json::ObjectMapper O(i);
        int64_t fileDataIndex;
        O.map("Index", fileDataIndex);

        if (dataIndex == fileDataIndex) {
            O.map("Name", dataInterval.fileName);
            O.map("Local", dataInterval.local);
        }
    }

    return dataInterval;
}

std::vector<ParameterInfo>  readParametersList(std::string databaseFName) {
    std::ifstream ifs(databaseFName);
    std::string content(
        (std::istreambuf_iterator<char>(ifs)),
        (std::istreambuf_iterator<char>())
    );

    auto e =json::parse(content);
    json::Value &jValue=e.get();

    json::Object* jObject          = jValue.getAsObject();
    json::Array*  parametersList   = (*jObject)["Parameters.List"].getAsArray();


    json::Object* parameter        = (*parametersList)[0].getAsObject();

    std::vector<ParameterInfo>  parameterInfoList;
    for (auto i : *parametersList) {
        json::Object   pObject=*i.getAsObject();
        ParameterInfo  parameterInfo;

        parameterInfo.parameterName     =   pObject[" Name"].getAsString().getValue();
        json::Array & DataFilesList     = *(pObject["Data.Files.List"].getAsArray());
        json::Array & DataFragmentsList = *(pObject["Data.Fragments.List"].getAsArray());


        for (auto k : DataFragmentsList) {
            DataInterval di= getDataInterval(k, DataFilesList);
            parameterInfo.intervalList.push_back(di);

        }

        parameterInfoList.push_back(parameterInfo);
    }
    return parameterInfoList;
}