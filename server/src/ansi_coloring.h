#include <initializer_list>
#include <string>

struct ColorVariants
{
    const unsigned short normal;
    const unsigned short bright;
};

struct ColorSpec
{
    const ColorVariants black;
    const ColorVariants red;
    const ColorVariants green;
    const ColorVariants yellow;
    const ColorVariants blue;
    const ColorVariants magenta;
    const ColorVariants cyan;
    const ColorVariants white;
    const ColorVariants normal;
};

struct ColorStyles
{
    const ColorSpec foreground;
    const ColorSpec background;
};

inline constexpr ColorStyles COLOR_STYLES = {
    .foreground = {
        .black = {.normal = 30, .bright = 90},
        .red = {.normal = 31, .bright = 91},
        .green = {.normal = 32, .bright = 92},
        .yellow = {.normal = 33, .bright = 93},
        .blue = {.normal = 34, .bright = 94},
        .magenta = {.normal = 35, .bright = 95},
        .cyan = {.normal = 36, .bright = 96},
        .white = {.normal = 37, .bright = 97},
        .normal = {.normal = 39, .bright = 39}
    },
    .background = {
        .black = {.normal = 40, .bright = 100},
        .red = {.normal = 41, .bright = 101},
        .green = {.normal = 42, .bright = 102},
        .yellow = {.normal = 43, .bright = 103},
        .blue = {.normal = 44, .bright = 104},
        .magenta = {.normal = 45, .bright = 105},
        .cyan = {.normal = 46, .bright = 106},
        .white = {.normal = 47, .bright = 107},
        .normal = {.normal = 49, .bright = 49}
    }
};

enum class FontStyles {
    BOLD = 1,
    FAINT = 2,
    ITALIC = 3,
    UNDERLINE = 4,
    BLINK_SLOW = 5,
    BLINK_FAST = 6,
    INVERT = 7,
    CONCEAL = 8,
    STRIKETHROUGH = 9,
    OVERLINED = 53
};

std::string STYLE_TEXT(const std::string&);
std::string STYLE_TEXT(const std::string&, const std::initializer_list<unsigned short>&);
