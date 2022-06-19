#ifndef EXPRESS_EX_H
#define EXPRESS_EX_H
#include <vector>
#include <map>
#include <string>
#include "parameterIO.h"
#include "ExStreamIfs.h"


#ifdef _MSC_VER 
#pragma warning( push )  
#pragma warning(disable : 5208)
#pragma warning( disable : 4251)
#pragma warning( disable : 4100)
#pragma warning( disable : 4189)
#endif 


class KEXParser;
class Body;
class BodyTemplate;
class Table;









DLL_EXPORT void  jit_init();


class DLL_EXPORT  Express_ex {
public :
    ~Express_ex();
    bool                                parseText(const std::string &str, bool is_file_name = false, std::map<std::string, bool/*is_file_name*/> lib_str_map ={});

    bool                                setParameters(const std::map<std::string, ParameterIfs*> &parameters_map);

    //  return map< name ,link name>
    std::map<std::string,std::string >  getParameterLinkNamesMap(bool hide_unused = false);
    std::vector<ParameterIfs*>          getOutputParameterVector();
    
    bool                                genJit(bool optimization_enable = true);
    bool                                run();

    const std::string &                 getErrorMsg() { return  error_str_; }

    void                                setErrorIo(ExStreamIfs* syntax);
    void                                setInfoStream(ExStreamIfs* info_stream);


    bool name_list_ = false;
    bool untyped_fsr_ = false;
    bool active_name_list_ = false;
    bool all_fsr_ = false;
    bool reduced_fsr_ = false;
    bool output_prm_ = false;
    bool table_ssr_ = false;
    bool llvm_ir_code_ = false;

    bool optimization_enable_ = true;
private:

    ExStreamIfs* info_stream_;



    KEXParser * kex_parser_ = nullptr;
    BodyTemplate* body_template_ = nullptr;
    Body*   body_ = nullptr;
    Table*  table_ = nullptr;

    std::string error_str_  ;


};


#ifdef _MSC_VER 
#pragma warning( pop )  
#endif 


#endif //EXPRESS_EX_H