//
// Created by SVK on 15.07.2022.
//

#ifndef EXPRESS_EX_ENV_EXCONSTARRAY_H
#define EXPRESS_EX_ENV_EXCONSTARRAY_H

#include "operations/ExValue_ifs.h"

class ExConstArray : public ExValue_ifs {
   public:
    ExConstArray(ExValue_ifs* arg_1, ExValue_ifs* arg_2, ExValue_ifs* arg_3);;

    ExConstArray(ExValue_ifs* arg_1, ExValue_ifs* arg_2);;

    explicit ExConstArray(ExValue_ifs* arg_1);;


   private:
    static std::string getTextValue(ExValue_ifs* arg_1, ExValue_ifs* arg_2 = nullptr) {
        if (arg_2 == nullptr) {
            return "range(" + std::to_string(arg_1->getBinaryValue()) + ")";
        } else if (isConst(arg_1) && isConst(arg_2) && isInteger(arg_1) && isInteger(arg_2))
            return "range(" + std::to_string(arg_1->getBinaryValue()) + "," + std::to_string(arg_2->getBinaryValue()) +
                   ")";
        return "none";
    }

    const std::string text_value_;
};



#endif  // EXPRESS_EX_ENV_EXCONSTARRAY_H
