#include "chalk.h"
#include <regex>
#include <string>

inline constexpr char CSI = '\x1b';
inline constexpr std::string CODE_START{CSI, '['};
inline constexpr std::string CODE_REGEX_START{CSI, '\\', '['};

std::string Chalk::operator()(const std::string& text) const
{
    if (this == &CHALK)
        return text;

    const std::string opening{CODE_START + std::to_string(this->m_opening_code) + 'm'};
    const std::string closing_code = std::to_string(this->m_closing_code) + 'm';
    const std::string closing{CODE_START + closing_code};
    const std::regex closing_regex{CODE_REGEX_START + closing_code};

    const std::string body{std::regex_replace(this->m_parent->operator()(text), closing_regex, opening)};

    return opening + body + closing;
}

consteval Chalk CREATE_INSTANCE(
    const unsigned short m_opening_code,
    const unsigned short m_closing_code,
    const Chalk* const m_parent
) noexcept
{
    return {m_opening_code, m_closing_code, m_parent};
}

const Chalk CHALK{CREATE_INSTANCE(0, 0, nullptr)};
