#include <catch.hpp>
#include <fstream>
#include <iostream>

TEST_CASE("FileEqual") {
    {
        std::ifstream input_file_stream("/Users/thenromanov/Projects/Archiver/tests/data/a.arc",
                                        std::ios_base::in | std::ios::binary);
        int j = 0;
        while (!input_file_stream.eof()) {
            uint16_t c = input_file_stream.get();
            if (input_file_stream.eof()) {
                break;
            }
            for (int i = 0; i < 8; ++i) {
                ++j;
                std::cout << ((c >> i) & 1) << ' ';
            }
            std::cout << "| ";
        }
        std::cout << '\n';
        input_file_stream.close();
    }
    {
        std::ifstream input_file_stream("/Users/thenromanov/Projects/Archiver/src/unit_tests/a.arc",
                                        std::ios_base::in | std::ios::binary);
        int j = 0;
        while (!input_file_stream.eof()) {
            uint16_t c = input_file_stream.get();
            if (input_file_stream.eof()) {
                break;
            }
            for (int i = 0; i < 8; ++i) {
                ++j;
                std::cout << ((c >> i) & 1) << ' ';
            }
            std::cout << "| ";
        }
        std::cout << '\n';
        input_file_stream.close();
    }
}