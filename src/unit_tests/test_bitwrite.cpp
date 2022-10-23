#include <catch.hpp>
#include <fstream>
#include <vector>

#include "bitwrite.hpp"

TEST_CASE("File writing") {
    std::fstream output_file_stream("../src/unit_tests/ouput_file.txt", std::ios_base::out | std::ios::binary);
    if (!output_file_stream.is_open()) {
        return;
    }

    BitWrite writer(output_file_stream);
    writer.Put(0, 1);
    writer.Put(0b110, 3);
    writer.Put(0b000101100, 9);
    writer.Put(0b01001100001, 11);
    writer.Flush();
    output_file_stream.close();

    std::fstream input_file_stream("../src/unit_tests/ouput_file.txt", std::ios_base::in | std::ios::binary);
    if (!input_file_stream.is_open()) {
        return;
    }
    std::vector<char> readen;
    while (!input_file_stream.eof()) {
        char c = static_cast<char>(input_file_stream.get());
        if (input_file_stream.eof()) {
            break;
        }
        readen.push_back(c);
    }

    const std::vector<char> result = {'a', 'b', 'a'};
    input_file_stream.close();
    REQUIRE(result == readen);
}

TEST_CASE("Invalid reading") {
    std::fstream output_file_stream("../src/unit_tests/ouput_file.txt", std::ios_base::out | std::ios::binary);
    if (!output_file_stream.is_open()) {
        return;
    }

    BitWrite writer(output_file_stream);
    try {
        writer.Put(1, 1000);
    } catch (const std::exception& e) {
        output_file_stream.close();
        return;
    }
    output_file_stream.close();
    FAIL("IllegalWriting");
}