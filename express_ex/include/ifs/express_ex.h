#ifndef EXPRESS_EX_H
#define EXPRESS_EX_H
#include <map>
#include <string>
#include <vector>

#include "ExStreamIfs.h"
#include "parameterIO.h"



class KEXParser;
class Body;
class BodyTemplate;
class Table;



DLL_EXPORT void jit_init();

/**
 * this is the main interface to deal with express.
 * Firstly you should use parseText, it makes primary syntax analysis.
 * Than you are able to get map of link names by using getParameterLinkNamesMap.
 * For next step of full semantic analysis Express_ex instance require input ParameterIfs instances via
 * setParameters() to determine types of parameters (sync/async, data type, time type ,frequency is exists)
 * and consequently types of all variables and outputs. After steps mentioned above it is possible to
 * fetch output parameters by getOutputParameters(). Now it is possible to configure parameters,
 * just edit received via getOutputParameters() instances of ParameterIfs. Next use genJit() to build
 * and link; and last step is run();
 *
 */
class DLL_EXPORT Express_ex {
   public:
    ~Express_ex();

    /**
     *
     * @param str main source code or filename of file with main source code
     * @param is_file_name
     * @param lib_str_list list of pairs."first" part of a pair is either a filename of library or a
     * source texts itself, "second" part of a pair is a bool value, which is true if "first" is filename
     * @return true if successful
     */
    bool parseText(const std::string& str, bool is_file_name = false,
                   std::list<std::pair<std::string, bool /*is_file_name*/>> lib_str_list = {});


    /**
     *
     * \returns map< name ,link name>. name - code name, link name - parameter name.
     * example: `x = param("#1_A01__1")`  where  x - name, #1_A01__1 - link name.
     */
    std::map<std::string, std::string> getParameterLinkNamesMap(bool hide_unused = false) const;


    /**
     * it runs step of full semantic analysis.
     * @param parameters_map map< name, corresponding parameter>.
     * @return true if successful
     */
    bool setParameters(const std::map<std::string, ParameterIfs*>& parameters_map);


    /**
     *
     * @return list of output parameters
     */
    std::list<ParameterIfs*> getOutputParameters() const;


    /**
     *
     * @param optimization_enable  pass true if you want enable optimizations
     * @return true if successful
     */
    bool genJit(bool optimization_enable = true);

    /**
     * runs calculation of parameters
     * @return true if successful
     */
    bool run();


    /**
     * set error_stream once to get error information at each step,
     * by default it doesnt have any error stream.
     * it doesn't take ownership of error_stream
     * @param error_stream output stream to which errors will be written
     */
    void setErrorIo(ExStreamIfs* error_stream);

    /**
     * set info_stream once to get build information at each step,
     * by default it doesnt have any info output stream.
     * it doesn't take ownership of info_stream
     * @param error_stream output stream to which info stream will be written
     */
    void setInfoStream(ExStreamIfs* info_stream);


    bool name_list_ = false;
    bool untyped_fsr_ = false;
    bool active_name_list_ = false;
    bool all_fsr_ = false;
    bool reduced_fsr_ = false;
    bool output_prm_ = false;
    bool table_ssr_ = false;
    bool llvm_ir_code_ = false;


   private:
    ExStreamIfs* info_stream_ = nullptr;
    ExStreamIfs* error_stream_ = nullptr;


    KEXParser* kex_parser_ = nullptr;
    BodyTemplate* body_template_ = nullptr;
    Body* body_ = nullptr;
    Table* table_ = nullptr;
};



#endif  // EXPRESS_EX_H