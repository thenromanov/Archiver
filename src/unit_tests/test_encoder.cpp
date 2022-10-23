#include <algorithm>
#include <catch.hpp>
#include <unordered_map>
#include <vector>

#include "encoder.hpp"

TEST_CASE("Encoding") {
    {
        std::unordered_map<uint16_t, size_t> frequency = {
            {static_cast<uint16_t>('a'), 2}, {static_cast<uint16_t>('c'), 1}, {static_cast<uint16_t>('b'), 10}};
        Encoder encoder;
        auto binary_lengths = encoder.GetCodesLengths(frequency);
        const std::vector<std::pair<uint16_t, size_t>> result = {
            {static_cast<uint16_t>('a'), 2}, {static_cast<uint16_t>('b'), 1}, {static_cast<uint16_t>('c'), 2}};
        std::sort(binary_lengths.begin(), binary_lengths.end());
        REQUIRE(binary_lengths == result);
        auto codes = encoder.GetCanonicalCodes<2>(binary_lengths);
        REQUIRE(codes[static_cast<uint16_t>('a')].GetSize() == 2);
        REQUIRE(codes[static_cast<uint16_t>('a')][0] == 0);
        REQUIRE(codes[static_cast<uint16_t>('a')][1] == 1);
        REQUIRE(codes[static_cast<uint16_t>('b')].GetSize() == 1);
        REQUIRE(codes[static_cast<uint16_t>('b')][0] == 0);
        REQUIRE(codes[static_cast<uint16_t>('c')].GetSize() == 2);
        REQUIRE(codes[static_cast<uint16_t>('c')][0] == 1);
        REQUIRE(codes[static_cast<uint16_t>('c')][1] == 1);
    }
}