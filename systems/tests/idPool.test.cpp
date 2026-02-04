#include "util/idPool.h"
#include <cstdint>
#include <doctest/doctest.h>

TEST_CASE("Id Pool")
{

    SUBCASE("Should get ID's properly")
    {
        IdPool<uint16_t> pool{5};

        CHECK(pool.getId() == 1);
        CHECK(pool.getId() == 2);
        CHECK(pool.getId() == 3);
        CHECK(pool.getId() == 4);
        CHECK(pool.getId() == 5);
        CHECK_FALSE(pool.hasIdsLeft());
    }

    SUBCASE("Should throw when out of IDs")
    {
        IdPool<uint16_t> pool{2};

        (void)pool.getId();
        (void)pool.getId();
        CHECK_FALSE(pool.hasIdsLeft());
        CHECK_THROWS_WITH_AS((void)pool.getId(), "Ran out of IDs!", std::runtime_error);
    }

    SUBCASE("Should be able to give IDs back")
    {
        IdPool<uint16_t> pool{2};

        (void)pool.getId();
        (void)pool.getId();
        CHECK_FALSE(pool.hasIdsLeft());
        pool.giveId(2);
        CHECK(pool.hasIdsLeft());
        CHECK(pool.getId() == 2);
    }

    SUBCASE("Should throw if Id is out of range")
    {
        IdPool<uint16_t> pool{2};

        CHECK_THROWS_WITH_AS(pool.giveId(3), "giveId: ID out of range", std::out_of_range);
    }
}
