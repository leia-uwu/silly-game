#include "common/src/ansi_coloring.h"
#include <iostream>

int main()
{
    std::cout << "be gay do crime\n"
              << "(boilerplate message lol)\n";

    std::cout << STYLE_TEXT("n" + STYLE_TEXT("y" + STYLE_TEXT("abo", {COLOR_STYLES.foreground.white.bright}) + "o", {COLOR_STYLES.foreground.magenta.bright}) + "m", {COLOR_STYLES.foreground.blue.bright})
              << '\n';

    return 0;
}
