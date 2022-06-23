


#include "ifs/parameterIO.h"
#include "printer.h"

#include "parser/body.h"
#include "parser/KexParser.h"
#include "parser/line.h"
#include "parser/bodyTemplate.h"
#include "ifs/express_ex.h"


#define REMOVE_MEMBER(X)    \
                            delete X;\
                            X = nullptr


Express_ex::~Express_ex() {
    REMOVE_MEMBER(table_);
    REMOVE_MEMBER(body_);
    REMOVE_MEMBER(kex_parser_);
}



bool Express_ex::parseText(const std::string &str, bool is_file_name,
                           std::map<std::string, bool/*is_file_name*/> lib_str_map) {

    //
    bool status = false;


    REMOVE_MEMBER(table_);
    REMOVE_MEMBER(body_);
    //REMOVE_MEMBER(body_template_);
    REMOVE_MEMBER(kex_parser_);


    try {
        kex_parser_ = new KEXParser(str, is_file_name, lib_str_map);

        body_template_ = kex_parser_->getActivBody();
        //body_template_->getParameterLinkNames(); TODO try to remove

        if (info_stream_) {
            std::string output;
            if (name_list_)
                *info_stream_ << Delimiter::GREEN << "names list: \n  " << body_template_->getParameterLinkNames()
                              << " \n";

            if (untyped_fsr_)
                *info_stream_ << Delimiter::GREEN << body_template_->print("") << Delimiter::GREEN << "\n";

            *info_stream_ << output;
        }


        status = true;
    }
    catch (size_t) {
        //TODO
        //error_str_ = g_error_str;
    }

    return status;
}


bool Express_ex::setParameters(const std::map<std::string, ParameterIfs *> &parameters_map) {
    bool status = false;


    stack<Value *> args;
    for (auto p: parameters_map)
        if (p.second != nullptr)
            args.push(new Line(p.first, p.second));
        else {
            if(error_stream_)
                *error_stream_<<ExColors::RED <<"Error: null_ptr in parameters_map \n"<< ExColors::RESET ;
            return false;
        }

    try {

        REMOVE_MEMBER(table_);
        REMOVE_MEMBER(body_);

        body_ = body_template_->genBodyByTemplate(nullptr, args, false);
        body_->symplyfy();

        //body_->getParameterLinkNames(true); TODO try to remove

        if (info_stream_) {
            if (active_name_list_)
                *info_stream_ << Delimiter::GREEN << "names list: \n  " << body_->getParameterLinkNames(true) << " \n";
            if (all_fsr_)
                *info_stream_ << Delimiter::GREEN << body_->print("");
            if (reduced_fsr_)
                *info_stream_ << Delimiter::GREEN << body_->print("", false, true);
        }


        table_ = new Table();
        auto context = TableGenContext(table_);

        body_->genTable(&context);

        int index = 0;
        for (auto i: body_->getOutputParameterList())
            i->setName("out_" + std::to_string(index++));

        if (info_stream_) {
            if (output_prm_) {
                *info_stream_ << Delimiter::GREEN << "input_prm:";
                for (auto i: args) {
                    *info_stream_ << Delimiter::GREEN << *(i->getParameter());
                }
                *info_stream_ << Delimiter::GREEN << "output_prm:";
                for (auto i: body_->getOutputParameterList()) {
                    *info_stream_ << Delimiter::GREEN << *i;
                }
            }
            if (table_ssr_)
                *info_stream_ << Delimiter::GREEN << table_->print();
        }


        status = true;
    }
    catch (size_t) {
        //TODO
        //error_str_ = g_error_str;
    }

    return status;
}

std::map<std::string, std::string> Express_ex::getParameterLinkNamesMap(bool hide_unused) {
    if (body_template_)
        return body_template_->getParameterLinkNames(hide_unused);
    else
        return {};
}

std::vector<ParameterIfs *> Express_ex::getOutputParameterVector() {
    if (body_)
        return body_->getOutputParameterList();
    else
        return {};
}

bool Express_ex::genJit(bool optimization_enable) {
    bool status = false;
    try {
        table_->calculateBufferLength();
        table_->llvmInit();
        table_->generateIR();
        if (optimization_enable)
            table_->runOptimization();

        if (info_stream_ && llvm_ir_code_)
            *info_stream_ << ExColors::GREEN << "\n\n---------We just constructed this LLVM module:--------- \n"
                          << ExColors::RESET << table_->printllvmIr();

        status = true;
    }
    catch (size_t) {
        //TODO
        //error_str_ = g_error_str;
    }

    return status;
}

bool Express_ex::run() {
    table_->run();
    return true;
}


//TODO: need to remove this
extern ExStreamIfs *g_error_stream;


void Express_ex::setErrorIo(ExStreamIfs *error_stream) {
    error_stream_ = error_stream;
    g_error_stream = error_stream;
}

void Express_ex::setInfoStream(ExStreamIfs *info_stream) {
    info_stream_ = info_stream;
}
