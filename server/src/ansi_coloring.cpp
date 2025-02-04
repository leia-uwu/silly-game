#include "ansi_coloring.h"
#include <regex>
#include <sstream>

template<typename RAI>
static std::string join(const RAI& begin, const RAI& end)
{
    std::stringstream res{};
    for (auto iter = begin; iter != end; ++iter) {
        res << std::to_string(*iter);
    }

    return res.str();
}

std::string STYLE_TEXT(const std::string& text)
{
    return text;
}

std::string STYLE_TEXT(const std::string& text, const std::initializer_list<unsigned short>& styles)
{
    static constexpr std::string CSI = "\x1b";
    static constexpr std::string OPENING_CODE = CSI + "[";
    static constexpr std::string CLOSING_CODE = OPENING_CODE + "0m";
    static const std::regex CLOSING_CODE_REGEX("\x1b\\[0m");

    const std::string opening = CSI + "[" + join(styles.begin(), styles.end()) + "m";

    return opening + std::regex_replace(text, CLOSING_CODE_REGEX, opening) + CSI + "[0m";
}
