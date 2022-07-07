


#include <clocale>

#include "DatasetsStorage_ifs.h"
#include "ifs/ExStream.h"
#include "ifs/express_ex.h"
#include "llvm/Support/CommandLine.h"


class DLL_EXPORT ParametersDB {
   public:
    explicit ParametersDB(const std::vector<std::string>& file_name_list) { addParametersSet(file_name_list); }


    ~ParametersDB() {
        for (const auto& u : list_)
            for (const auto& i : u.db_parameters) delete i;
    }

    bool addParametersSet(const std::vector<std::string>& file_name_list) {
        auto ret = true;
        for (auto& file_name : file_name_list) {
            DbUnit unit;
            unit.storage = openDatasetsStorage(file_name, true);

            std::string name = llvm::sys::path::filename(file_name).str();
            if (llvm::sys::path::extension(file_name) == ".h5") name = "__$Header";

            ret &= readParametersList(unit.storage.get(), name, unit.db_parameters);
            list_.push_back(std::move(unit));
        }
        return ret;
    }

    ParameterIfs* operator[](const std::string& name) {
        for (const auto& u : list_)
            for (const auto& i : u.db_parameters)
                if (i->getName() == name) return i;
        return nullptr;
    }

   private:
    struct DbUnit {
        std::unique_ptr<DatasetsStorage_ifs> storage;
        std::vector<ParameterIfs*> db_parameters;
    };

    std::list<DbUnit> list_;
};


int main(int argc, const char* argv[]) {
    std::setlocale(LC_ALL, "en_US.UTF-8");

    enum ShowNames
    {
        nameList,
        untypedFSR,
        activeNameList,
        allFSR,
        redusedFSR,
        tableSSR,
        outputPrm,
        llvmIRcode
    };


    static llvm::cl::OptionCategory mainCategory("Compiler Options", "Main Options ");

    static llvm::cl::opt<std::string> inputFile("i", llvm::cl::desc("input source file"),
                                                llvm::cl::value_desc("filename"), llvm::cl::Required,
                                                llvm::cl::cat(mainCategory));

    static llvm::cl::opt<std::string> outputFile("o", llvm::cl::desc("output data file name"),
                                                 llvm::cl::value_desc("filename"), llvm::cl::init("out.dat"),
                                                 llvm::cl::Optional, llvm::cl::cat(mainCategory));

    static llvm::cl::opt<bool> optimizationEnable("opt", llvm::cl::desc("optimization enable"), llvm::cl::Optional,
                                                  llvm::cl::cat(mainCategory));

    static llvm::cl::opt<bool> ansiEscapeCodes("ansi", llvm::cl::desc("enable ANSI escape codes"), llvm::cl::Optional,
                                               llvm::cl::cat(mainCategory));

    static llvm::cl::list<std::string> libraryPaths("dir", llvm::cl::desc("list of available directories"),
                                                    llvm::cl::value_desc("filename"), llvm::cl::ZeroOrMore,
                                                    llvm::cl::cat(mainCategory));

    static llvm::cl::list<std::string> moduleFiles("lib", llvm::cl::desc("list of available modules"),
                                                   llvm::cl::value_desc("filename"), llvm::cl::ZeroOrMore,
                                                   llvm::cl::cat(mainCategory));

    static llvm::cl::list<std::string> inputDataBaseFile("db", llvm::cl::desc("input data base files"),
                                                         llvm::cl::value_desc("path"), llvm::cl::ZeroOrMore,
                                                         llvm::cl::cat(mainCategory));

    static llvm::cl::opt<bool> runJit("runJit", llvm::cl::desc("run jit"), llvm::cl::Optional,
                                      llvm::cl::cat(mainCategory));

    static llvm::cl::bits<ShowNames> showBits(
        llvm::cl::desc("show:"), llvm::cl::cat(mainCategory), llvm::cl::ZeroOrMore,
        llvm::cl::values(clEnumVal(nameList, "names list"),
                         clEnumVal(untypedFSR, "FSR(first stage representation) without type calculation"),
                         clEnumVal(activeNameList, "Procedure Integration"),
                         clEnumVal(allFSR, "full FSR(first stage representation)"),
                         clEnumVal(redusedFSR, "redused FSR code"),
                         clEnumVal(tableSSR, "table (second stage) representation"),
                         clEnumVal(outputPrm, "output parameters"), clEnumVal(llvmIRcode, "llvm IR ")));

    llvm::cl::SetVersionPrinter([](llvm::raw_ostream& OS) { OS << "version - 0.94_6\n"; });
    llvm::cl::HideUnrelatedOptions(mainCategory);
    llvm::cl::ParseCommandLineOptions(argc, argv, "express jit");



    // g_ansi_escape_codes = ansiEscapeCodes;

    ParametersDB parameter_db(inputDataBaseFile);

    auto storage = openDatasetsStorage(llvm::sys::path::parent_path(inputDataBaseFile[0]).str(), true);


    ExStream ex_stream;
    ExErrorStream ex_error_stream;

    if (0 == inputDataBaseFile.size()) {
        ex_error_stream << ExColors::RED << "there are no input database file\n" << ExColors::RESET;
        return 1;
    }

    Express_ex express_ex;

    express_ex.setErrorIo(&ex_error_stream);
    express_ex.setInfoStream(&ex_stream);

    express_ex.name_list_ = showBits.isSet(nameList);
    express_ex.untyped_fsr_ = showBits.isSet(untypedFSR);
    express_ex.active_name_list_ = showBits.isSet(activeNameList);
    express_ex.all_fsr_ = showBits.isSet(allFSR);
    express_ex.reduced_fsr_ = showBits.isSet(redusedFSR);
    express_ex.output_prm_ = showBits.isSet(outputPrm);
    express_ex.llvm_ir_code_ = showBits.isSet(llvmIRcode);



    std::list<std::pair<std::string, bool /*is_file_name*/>> modules_map;
    for (const auto& i : moduleFiles) modules_map.push_back({i, true});

    if (!express_ex.parseText(inputFile, true, modules_map)) return 1;

    auto parameter_name_list = express_ex.getParameterLinkNamesMap();

    std::map<std::string, ParameterIfs*> parameters_map;
    for (const auto& i : parameter_name_list) {
        parameters_map[i.first] = parameter_db[i.second];
    }


    if (!express_ex.setParameters(parameters_map)) return 1;

    auto list = express_ex.getOutputParameters();



    llvm::SmallString<5> small_string(outputFile);
    llvm::sys::path::replace_extension(small_string, "");
    std::string extension_less_output_file_path = small_string.str().str();
    std::string output_extension = llvm::sys::path::extension(outputFile).str();



    auto output_storage = openDatasetsStorage(llvm::sys::path::parent_path(outputFile).str(), true);

    int index = 0;
    if (list.size() == 1) {
        list.front()->setName(output_storage.get(), extension_less_output_file_path, output_extension);
    } else
        for (auto& i : express_ex.getOutputParameters()) {
            i->setName(output_storage.get(), outputFile + std::to_string(index++), output_extension);
        }


    jit_init();
    if (!express_ex.genJit(optimizationEnable)) return 1;

    if (runJit) express_ex.run();


    ex_stream << "complete\n";


    return 0;
}
