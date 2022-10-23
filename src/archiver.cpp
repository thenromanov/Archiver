#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "bitread.hpp"
#include "bitwrite.hpp"
#include "cli_parser.hpp"
#include "encoder.hpp"

const uint16_t FILENAME_END = 256;
const uint16_t ONE_MORE_FILE = 257;
const uint16_t ARCHIVE_END = 258;

const uint16_t MAX_CODE_LENGTH = 257;

void Encode(std::string_view output_path, const std::vector<std::string_view>& input_paths) {
    std::ofstream output_file_stream(output_path, std::ios_base::out | std::ios::binary);
    if (!output_file_stream.is_open()) {
        throw std::runtime_error("Can't open file");
    }

    BitWrite writer(output_file_stream);

    for (auto input_path : input_paths) {
        size_t name_start = input_path.rfind('/', input_path.back()) + 1;
        std::string_view file_name = input_path.substr(name_start, input_path.size() - name_start);
        std::ifstream input_file_stream(input_path, std::ios_base::in | std::ios::binary);
        if (!input_file_stream.is_open()) {
            throw std::runtime_error("Can't open file");
        }

        std::unordered_map<uint16_t, size_t> frequency;
        frequency[FILENAME_END] = 1;
        frequency[ONE_MORE_FILE] = 1;
        frequency[ARCHIVE_END] = 1;

        for (uint16_t ch : file_name) {
            ++frequency[ch];
        }

        BitRead reader(input_file_stream);
        while (!reader.IsFinished()) {
            ++frequency[reader.Get(8)];
        }
        input_file_stream.close();

        Encoder encoder;
        std::vector<std::pair<uint16_t, size_t>> binary_lengths = encoder.GetCodesLengths(frequency);
        std::sort(binary_lengths.begin(), binary_lengths.end(), [](auto first, auto second) {
            if (first.second != second.second) {
                return first.second < second.second;
            }
            return first.first < second.first;
        });
        std::unordered_map<uint16_t, Bitarray<MAX_CODE_LENGTH>> symbol_code =
            encoder.GetCanonicalCodes<MAX_CODE_LENGTH>(binary_lengths);

        std::vector<size_t> sizes_amount(binary_lengths.back().second);
        for (auto [symbol, length] : binary_lengths) {
            ++sizes_amount[length - 1];
        }

        writer.Put(binary_lengths.size(), 9);
        for (auto [symbol, length] : binary_lengths) {
            writer.Put(symbol, 9);
        }
        for (size_t size : sizes_amount) {
            writer.Put(size, 9);
        }
        for (uint16_t ch : file_name) {
            auto it = symbol_code.find(ch);
            for (size_t i = it->second.GetSize(); i > 0; --i) {
                writer.Put(it->second[i - 1], 1);
            }
        }
        auto filename_it = symbol_code.find(FILENAME_END);
        for (size_t i = filename_it->second.GetSize(); i > 0; --i) {
            writer.Put(filename_it->second[i - 1], 1);
        }

        input_file_stream.open(input_path, std::ios_base::in | std::ios::binary);
        reader.ChangeStream(input_file_stream);
        while (!reader.IsFinished()) {
            uint16_t ch = reader.Get(8);
            auto it = symbol_code.find(ch);
            for (size_t i = it->second.GetSize(); i > 0; --i) {
                writer.Put(it->second[i - 1], 1);
            }
        }
        input_file_stream.close();

        uint16_t end_command = (input_path == input_paths.back() ? ARCHIVE_END : ONE_MORE_FILE);
        auto command_it = symbol_code.find(end_command);
        for (size_t i = command_it->second.GetSize(); i > 0; --i) {
            writer.Put(command_it->second[i - 1], 1);
        }
    }

    writer.Flush();
    output_file_stream.close();
}

int main(int argc, char** argv) {
    CLIParser parser;
    auto help_command = parser.AddArgument("-h", " -- list available subcommands and some concept guides.", 0, 0);
    auto zip_command = parser.AddArgument(
        "-c", "<archive_name file1 [file2 ...]> -- compresses the transferred files into an archive", 2);
    auto unzip_command = parser.AddArgument("-d", "<archive_name> -- decompresses given archive", 1, 1);
    try {
        auto arguments = parser.Parse(argc, argv);
        if (help_command) {
            std::cout << parser.GetData();
        } else if (zip_command) {
            std::string_view output_path = arguments[0];
            std::vector<std::string_view> input_paths(arguments.begin() + 1, arguments.end());
            Encode(output_path, input_paths);
        } else if (unzip_command) {
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 111;
    }
    return 0;
}
