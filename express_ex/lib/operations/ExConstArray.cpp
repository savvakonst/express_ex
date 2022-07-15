//
// Created by SVK on 15.07.2022.
//

#include "operations/ExConstArray.h"
ExConstArray::ExConstArray(ExValue_ifs *arg_1, ExValue_ifs *arg_2, ExValue_ifs *arg_3)
    : ExValue_ifs(TypeEn::double_jty, TypeEn::unknown_jty, DataStructureTypeEn::kSmallArr) {
    if (!(isConst(arg_1) && isConst(arg_2) && isConst(arg_3) && isInteger(arg_3))) {
        print_error("range args must be a constant");
        return;
        // length_ = arg_3->getBinaryValue();
        // type_ = TypeEn::double_jty;
        // start_ = arg_1->getDoubleValue();
        // stop_ = arg_2->getDoubleValue();
    } else {
        print_error("range(start_num,stop_num,length) -signature is not supported yet");
        return;
    }
}
ExConstArray::ExConstArray(ExValue_ifs *arg_1, ExValue_ifs *arg_2)
    : ExValue_ifs(TypeEn::int64_jty, TypeEn::unknown_jty, DataStructureTypeEn::kSmallArr),
      text_value_(getTextValue(arg_1, arg_2)) {
    if (isConst(arg_1) && isConst(arg_2) && isInteger(arg_1) && isInteger(arg_2)) {
        length_ = arg_2->getBinaryValue() - arg_1->getBinaryValue();
        data_structure_type_ = DataStructureTypeEn::kSmallArr;
        type_ = TypeEn::int64_jty;
        // text_value_ = "range(" + std::to_string(arg_1->getBinaryValue()) + "," +
        //               std::to_string(arg_2->getBinaryValue()) + ")";
        start_ = (double)arg_1->getBinaryValue();
        stop_ = (double)arg_2->getBinaryValue();
    } else {
        print_error("range(start_num,stop_num) - arg must be integer consant");
    }
}
ExConstArray::ExConstArray(ExValue_ifs *arg_1)
    : ExValue_ifs(TypeEn::int64_jty, TypeEn::unknown_jty, DataStructureTypeEn::kSmallArr),
      text_value_(getTextValue(arg_1)) {
    if (isConst(arg_1) && isInteger(arg_1)) {
        length_ = arg_1->getBinaryValue();
        data_structure_type_ = DataStructureTypeEn::kSmallArr;
        type_ = TypeEn::int64_jty;
        // text_value_ = "range(" + std::to_string(arg_1->getBinaryValue()) + ")";
        start_ = 0.0;
        stop_ = (double)length_;
    } else {
        print_error("range(len) - arg must be integer consant");
    }
}
