#include <catch.hpp>

#include "bitarray.hpp"

TEST_CASE("Constructors") {
    {
        Bitarray<11> b;
        REQUIRE(b.GetSize() == 1);
        REQUIRE(b[0] == false);
    }

    {
        Bitarray<100> a(11, 11);
        Bitarray<100> b(a);
        REQUIRE(b.GetSize() == 11);
        REQUIRE(b[0] == true);
    }
    {
        Bitarray<100> a(11, 11);
        Bitarray<100> b = a;
        REQUIRE(b.GetSize() == 11);
        REQUIRE(b[3] == true);
    }
    {
        Bitarray<100> a(11, 11);
        Bitarray<100> b = std::move(a);
        REQUIRE(b.GetSize() == 11);
        REQUIRE(b[3] == true);
    }
    {
        try {
            Bitarray<1> a(10, 1);
        } catch (const std::exception& e) {
            return;
        }
        FAIL("IllegalConstucion");
    }
}

TEST_CASE("Operations") {
    {
        Bitarray<100> a(11, 11);
        ++a;
        REQUIRE(a[0] == false);
        REQUIRE(a[1] == false);
        REQUIRE(a[2] == true);
        REQUIRE(a[3] == true);
        REQUIRE(a.GetSize() == 11);
    }
    {
        Bitarray<100> a(11, 11);
        a <<= 2;
        REQUIRE(a[0] == false);
        REQUIRE(a[1] == false);
        REQUIRE(a[2] == true);
        REQUIRE(a[3] == true);
        REQUIRE(a[4] == false);
        REQUIRE(a[5] == true);
        REQUIRE(a.GetSize() == 13);
    }
}