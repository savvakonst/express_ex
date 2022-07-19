#include "parser/basic.h"

#include "operations/ExValue_ifs.h"
#include "parser/line.h"

void PrintBodyContext::createArg(ExLine* value) {
    if (!hide_unused_lines_ || !value->isUnused()) {
        auto txt_line = "  " + tab_ + value->getName() + "=arg()";
        auto txt_shifts =
            std::to_string(value->getLeftBufferLen()) + " : " + std::to_string(value->getRightBufferLen());
        auto txt_skip =
            std::string(max_line_length_ - ((txt_line.length() > max_line_length_) ? 0 : txt_line.length()), ' ');
        result_ += txt_line + txt_skip + txt_shifts + "\n";
    }
}

void PrintBodyContext::createLine(ExLine* value) {
    auto DST_postfix = DST_ena_ ? "." + toString(value->ds_ty_) : "";

    if (!hide_unused_lines_ || !value->isUnused()) {
        const std::string txt_line = "  " + tab_ + value->getName() + DST_postfix + "=" + pop();
        const std::string txt_shifts = std::to_string(value->getLeftBufferLen()) + " : " +
                                       std::to_string(value->getRightBufferLen()) + " : " +
                                       std::to_string(value->getLength());
        const std::string txt_skip = std::string(
            max_line_length_ - ((txt_line.length() > max_line_length_) ? max_line_length_ - 2 : txt_line.length()),
            ' ');
        result_ += txt_line + txt_skip + txt_shifts + "\n";
    }
}

void PrintBodyContext::createReturn(ExLine* value) {
    auto DST_postfix = DST_ena_ ? "." + toString(value->ds_ty_) : "";

    auto txt_line = "  " + tab_ + value->getName() + DST_postfix + "  " + pop();
    auto txt_shifts = std::to_string(value->getLeftBufferLen()) + " : " + std::to_string(value->getRightBufferLen());
    auto txt_skip =
        std::string(max_line_length_ - ((txt_line.length() > max_line_length_) ? 0 : txt_line.length()), ' ');
    result_ += txt_line + txt_skip + txt_shifts + "\n";
}

void PrintBodyContext::setName(const std::string& name) { name_ = name; }