#include <catch.hpp>
#include <fstream>
#include <vector>

#include "bitread.hpp"

TEST_CASE("Binary reading") {
    std::fstream input_file_stream("../src/unit_tests/input_file.txt", std::ios_base::in | std::ios::binary);
    if (!input_file_stream.is_open()) {
        return;
    }

    BitRead reader(input_file_stream);
    const std::vector<uint16_t> result{0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1};
    std::vector<uint16_t> readen;
    while (!reader.IsFinished()) {
        readen.push_back(reader.Get(1));
    }
    input_file_stream.close();
    REQUIRE(result == readen);
}

TEST_CASE("Ninefold reading") {
    std::fstream input_file_stream("../src/unit_tests/sample_file.txt", std::ios_base::in | std::ios::binary);
    if (!input_file_stream.is_open()) {
        return;
    }

    BitRead reader(input_file_stream);
    const std::vector<uint16_t> result{0b011000010, 0b110001001, 0b100001000};
    std::vector<uint16_t> readen;
    while (!reader.IsFinished()) {
        readen.push_back(reader.Get(9));
    }
    input_file_stream.close();
    REQUIRE(result == readen);
}

TEST_CASE("Variant reading") {
    std::fstream input_file_stream("../src/unit_tests/sample_file.txt", std::ios_base::in | std::ios::binary);
    if (!input_file_stream.is_open()) {
        return;
    }

    BitRead reader(input_file_stream);
    const std::vector<uint16_t> result{0b0110000101100010, 0b0110000, 0b1};
    std::vector<uint16_t> readen;
    readen.push_back(reader.Get(16));
    readen.push_back(reader.Get(7));
    readen.push_back(reader.Get(1));
    input_file_stream.close();
    REQUIRE(result == readen);
}

TEST_CASE("Invalid reading") {
    std::fstream input_file_stream("../src/unit_tests/sample_file.txt", std::ios_base::in | std::ios::binary);
    if (!input_file_stream.is_open()) {
        return;
    }

    std::vector<uint16_t> readen;
    BitRead reader(input_file_stream);
    try {
        readen.push_back(reader.Get(1000));
    } catch (const std::exception& e) {
        return;
    }
    FAIL("IllegalReading");
}