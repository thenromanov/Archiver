#include <catch.hpp>

#include "bitarray.hpp"

TEST_CASE("Constructors") {
    {
        Bitarray<100> barray;
        REQUIRE(barray.GetSize() == 1);
        REQUIRE(barray[0] == 0);
    }
}