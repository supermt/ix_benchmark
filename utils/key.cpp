//
// Created by jinghuan on 1/3/21.
//

#include "include/format.h"
#include "slice.cpp"

namespace IX_NAME_SPACE {
    std::string Key::to_string() {
        std::string result;
        if (_type == kDouble) {
            std::ostringstream strs;
            strs << "double " << value;
            result = strs.str();
        } else {
            result = slice_value.ToString();
        }

        return result;
    };
};
