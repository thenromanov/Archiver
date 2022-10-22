#include <algorithm>
#include <bitset>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "bitarray.hpp"
#include "bitread.hpp"
#include "bitwrite.hpp"
#include "encoder_trie.hpp"

const uint16_t FILENAME_END = 256;
const uint16_t ONE_MORE_FILE = 257;
const uint16_t ARCHIVE_END = 258;

const uint16_t MAX_CODE_LENGTH = 257;

void Encode(std::ostream& output_stream, std::string_view file_path, uint16_t end_symbol) {
    size_t name_start = file_path.rfind('/', file_path.back()) + 1;
    std::string_view file_name = file_path.substr(name_start, file_path.size() - name_start);

    std::ifstream file(file_path, std::ios_base::in | std::ios::binary);
    if (!file.is_open()) {
        throw std::invalid_argument("Can't access given file");
    }

    std::unordered_map<uint16_t, size_t> frequency;
    frequency[FILENAME_END] = 1;
    frequency[ONE_MORE_FILE] = 1;
    frequency[ARCHIVE_END] = 1;
    for (auto ch : file_name) {
        ++frequency[ch];
    }
    BitRead reader(file);
    while (!reader.IsFinished()) {
        ++frequency[reader.Get(8)];
    }
    file.close();

    EncoderTrie trie;
    trie.BuildTrie(frequency);
    std::vector<std::pair<uint16_t, size_t>> binary_lengths = trie.GetLengths();

    std::sort(binary_lengths.begin(), binary_lengths.end(), [](auto first, auto second) {
        if (first.second != second.second) {
            return first.second < second.second;
        }
        return first.first < second.first;
    });
    const size_t max_code_size = binary_lengths.back().second;
    std::vector<size_t> sizes_amount(max_code_size);

    std::unordered_map<uint16_t, Bitarray<MAX_CODE_LENGTH>> symbol_code;
    Bitarray<MAX_CODE_LENGTH> current_code;

    symbol_code[binary_lengths.front().first] = current_code;
    ++sizes_amount[binary_lengths.front().second - 1];
    for (size_t i = 1; i < binary_lengths.size(); ++i) {
        ++current_code;
        current_code <<= (binary_lengths[i].second - binary_lengths[i - 1].second);
        symbol_code[binary_lengths[i].first] = current_code;
        ++sizes_amount[binary_lengths[i].second - 1];
    }

    BitWrite writer(output_stream);
    writer.Put(binary_lengths.size(), 9);
    for (auto [symbol, length] : binary_lengths) {
        writer.Put(symbol, 9);
    }
    for (size_t size : sizes_amount) {
        writer.Put(size, 9);
    }
    for (auto ch : file_name) {
        auto it = symbol_code.find(ch);
        for (size_t i = 0; i < it->second.GetSize(); ++i) {
            writer.Put(it->second[i], 1);
        }
    }
    auto filename_end_it = symbol_code.find(FILENAME_END);
    for (size_t i = 0; i < filename_end_it->second.GetSize(); ++i) {
        writer.Put(filename_end_it->second[i], 1);
    }
    file.open(file_path, std::ios_base::in | std::ios::binary);
    if (!file.is_open()) {
        throw std::invalid_argument("Can't access given file");
    }
    reader.ChangeStream(file);
    while (!reader.IsFinished()) {
        uint16_t symbol = reader.Get(8);
        auto it = symbol_code.find(symbol);
        for (size_t i = 0; i < it->second.GetSize(); ++i) {
            writer.Put(it->second[i], 1);
        }
    }
    file.close();
}

int main(int argc, char** argv) {
    return 0;
}
