#include "formatter.h"
#include <regex>
#include <string>

inline constexpr char CSI = '\x1b';
inline constexpr std::string CODE_START{CSI, '['};
inline constexpr std::string CODE_REGEX_START{CSI, '\\', '['};

std::string Formatter::operator()(const std::string& text) const
{
    if (this == &FORMATTER)
        return text;

    const std::string opening{CODE_START + std::to_string(this->m_openingCode) + 'm'};
    const std::string closing_code = std::to_string(this->m_closingCode) + 'm';
    const std::string closing{CODE_START + closing_code};
    const std::regex closing_regex{CODE_REGEX_START + closing_code};

    const std::string body{std::regex_replace(this->m_parent->operator()(text), closing_regex, opening)};

    return opening + body + closing;
}

consteval Formatter CreateInstance(
    const unsigned short m_openingCode,
    const unsigned short m_closingCode,
    const Formatter* const m_parent
) noexcept
{
    return {m_openingCode, m_closingCode, m_parent};
}

const Formatter FORMATTER{CreateInstance(0, 0, nullptr)};
