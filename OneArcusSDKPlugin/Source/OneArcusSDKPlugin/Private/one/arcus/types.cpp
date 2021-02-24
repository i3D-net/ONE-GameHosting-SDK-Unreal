// Copyright i3D.net, 2021. All Rights Reserved.
#include "types.h"

namespace i3d {
namespace one {

std::string to_std_string(const String &str) {
    return std::string(str.c_str());
}

String to_one_string(const std::string &str) {
    return String(str.c_str());
}

}  // namespace one
}  // namespace i3d