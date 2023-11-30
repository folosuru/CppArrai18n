#ifndef CPPARRAI18N_INCLUDE_CPPARRAI18N_ARRAI18N_DEF_HPP_
#define CPPARRAI18N_INCLUDE_CPPARRAI18N_ARRAI18N_DEF_HPP_
#include <string>
#include <vector>

/**
 * This file is only definition.
 *
 * if you want to use header-only library, include "Arrai18n.hpp"
 * if you want to use static library, include this file
 */

namespace Arrai18n {
using lang_name = std::string;
using key_name = std::string;
using args_list = std::vector<std::string>;

struct trl_text {
    key_name key;
    args_list args;
};

inline void load(const std::string&);

inline void setDefaultLanguage(const lang_name&);

inline std::string trl(const lang_name& lang_, const std::string& text, const std::vector<std::string>&  args = {});

inline std::string trl(const lang_name& lang_, const trl_text& text);

}
#endif //CPPARRAI18N_INCLUDE_CPPARRAI18N_ARRAI18N_DEF_HPP_
