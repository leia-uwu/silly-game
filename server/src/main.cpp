#include "common/src/chalk.h"
#include <iostream>

int main()
{
    std::cout << "be " << CHALK.bold().underline("gay") << " do " << CHALK.red().italic()("crime") << '\n'
              << "(boilerplate message lol)\n";

    std::cout << CHALK.blue_bright("n" + CHALK.magenta_bright("y" + CHALK.white_bright("abo") + "o") + "m") << '\n';

    return 0;
}
