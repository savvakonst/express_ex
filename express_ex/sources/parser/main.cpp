
#include <clocale>
#include <fstream>
#include <iostream>
#include <strstream>

#include "ifs/ParameterIO.h"
#include "ifs/express_ex.h"
#include "ifs/printer.h"
#include "llvm/Support/CommandLine.h"
#include "parser/body.h"
#include "parser/defWarningIgnore.h"
#include "parser/treeShapeListener.h"
#include "parser/undefWarningIgnore.h"

bool g_ansiEscapeCodes;

void clGpuConvolve();
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
                                                         llvm::cl::value_desc("directory"), llvm::cl::ZeroOrMore,
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

    // clGpuConvolve();

    g_ansiEscapeCodes = ansiEscapeCodes;

    ExColors colorReset = ExColors::RESET;
    ExColors colorRed   = ExColors::RED;
    ExColors colorGreen = ExColors::GREEN;
    Delimiter delimiter = Delimiter::GREEN;

    ParametersDB parameterDB(inputDataBaseFile);

    try {
        std::map<std::string, bool> modules_map;
        for (auto i : moduleFiles) modules_map[i] = true;

        KEXParser parser(inputFile, true, modules_map);
        BodyTemplate* body_template = parser.getActivBody();

        std::map<std::string, std::string> parameterNameList = body_template->getParameterLinkNames();

        if (showBits.isSet(nameList)) llvm::outs() << delimiter << "names list: \n  " << parameterNameList << " \n";

        stack<Value*> args;
        for (auto i : parameterNameList) {
            auto p = parameterDB[i.second];
            // llvm::outs() << delimiter << "Parameter: \n  " << *p << " \n";
            if (p != nullptr) {
                p->setLocal(false);
                args.push(new Line(i.first, p));  // potential error
            } else
                print_error("can not find parameter " + i.second);
        }
        if (showBits.isSet(untypedFSR)) llvm::outs() << delimiter << body_template->print("") << delimiter << "\n";

        if (0 == inputDataBaseFile.size()) {
            llvm::outs() << colorRed << "there are no input database file\n" << colorReset;
            return 1;
        }

        Body* body = body_template->genBodyByTemplate(nullptr, args, false);
        body->symplyfy();

        /// print block
        if (showBits.isSet(activeNameList))
            llvm::outs() << delimiter << "names list: \n  " << body->getParameterLinkNames(true) << " \n";

        if (showBits.isSet(allFSR)) llvm::outs() << delimiter << body->print("");

        if (showBits.isSet(redusedFSR)) llvm::outs() << delimiter << body->print("", false, true);

        jit_init();
        Table* table            = new Table();
        TableGenContext context = TableGenContext(table);

        body->genTable(&context);

        int index = 0;
        for (auto i : body->getOutputParameterList()) {
            i->setName("out_" + std::to_string(index++));
            i->setLocal(false);
        }
        if (showBits.isSet(outputPrm)) {
            llvm::outs() << delimiter << "input_prm:";
            for (auto i : args) {
                llvm::outs() << delimiter << *(i->getPatameter());
            }
            llvm::outs() << delimiter << "output_prm:";
            for (auto i : body->getOutputParameterList()) {
                llvm::outs() << delimiter << *i;
            }
        }

        if (showBits.isSet(tableSSR)) llvm::outs() << delimiter << table->print();

        table->calculateBufferLength();
        table->llvmInit();
        table->generateIR();

        if (optimizationEnable) table->runOptimization();

        if (showBits.isSet(llvmIRcode)) table->printllvmIr();

        if (runJit) table->run();

        llvm::outs() << "complete";
    } catch (size_t) {
    }

    return 0;
}
