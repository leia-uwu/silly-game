#include "util/idPool.h"

#include <cassert>
#include <cstdint>
#include <stdexcept>

int main()
{
    IdPool<uint16_t> pool{5};

    assert(pool.getId() == 1);
    assert(pool.getId() == 2);
    assert(pool.getId() == 3);
    assert(pool.getId() == 4);
    assert(pool.getId() == 5);

    pool.giveId(4);

    assert(pool.getId() == 4);

    bool didThrow = false;
    try {
        uint16_t id = pool.getId();
    } catch (std::runtime_error& e) {
        didThrow = true;
    }

    assert(didThrow);
}
