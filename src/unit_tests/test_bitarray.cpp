#include <catch.hpp>

#include "bitarray.hpp"

TEST_CASE("Constructors") {
    {
        Bitarray<100> barray(1, 0);
        REQUIRE(barray.GetSize() == 1);
        REQUIRE(barray[0] == 0);
    }
}

TEST_CASE("Set value") {
    {
        Bitarray<100> barray(10);
        barray[2] = 1;
        REQUIRE(barray[2] == 1);

        barray[0] = 1;
        barray[1] = 0;
        barray[0] = barray[1];

        REQUIRE(barray[0] == 0);

        barray[0] = 1;
        REQUIRE(barray[0] == 1);
    }
}