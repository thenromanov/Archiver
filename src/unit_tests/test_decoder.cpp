#include <algorithm>
#include <catch.hpp>
#include <unordered_map>
#include <vector>

#include "decoder.hpp"
#include "encoder.hpp"

TEST_CASE("Decoding") {
    {
        std::unordered_map<uint16_t, size_t> frequency = {
            {static_cast<uint16_t>('a'), 2}, {static_cast<uint16_t>('c'), 1}, {static_cast<uint16_t>('b'), 10}};
        Encoder encoder;
        Decoder decoder;
        auto binary_lengths = encoder.GetCodesLengths(frequency);
        const std::vector<std::pair<uint16_t, size_t>> result = {
            {static_cast<uint16_t>('a'), 2}, {static_cast<uint16_t>('b'), 1}, {static_cast<uint16_t>('c'), 2}};

        auto codes = decoder.GetCanonicalCodes<2>(binary_lengths);
        REQUIRE(codes[static_cast<uint16_t>('a')].GetSize() == 2);
        REQUIRE(codes[static_cast<uint16_t>('a')][0] == 0);
        REQUIRE(codes[static_cast<uint16_t>('a')][1] == 1);
        REQUIRE(codes[static_cast<uint16_t>('b')].GetSize() == 1);
        REQUIRE(codes[static_cast<uint16_t>('b')][0] == 0);
        REQUIRE(codes[static_cast<uint16_t>('c')].GetSize() == 2);
        REQUIRE(codes[static_cast<uint16_t>('c')][0] == 1);
        REQUIRE(codes[static_cast<uint16_t>('c')][1] == 1);

        for (auto& [symbol, code] : codes) {
            decoder.InsertBitarry(symbol, code);
        }

        REQUIRE(decoder.AutomatonStep(false) == static_cast<uint16_t>('b'));
        REQUIRE(decoder.AutomatonStep(true) == Decoder::EMPTY_SYMBOL);
        REQUIRE(decoder.AutomatonStep(true) == static_cast<uint16_t>('c'));
    }
}

TEST_CASE("Undefined behaviour") {
    {
        Decoder decoder;
        try {
            decoder.AutomatonStep(false);
        } catch (const std::exception& e) {
            return;
        }
        FAIL("IllegalOperation");
    }
}