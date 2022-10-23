#pragma once

#include <algorithm>
#include <unordered_map>
#include <vector>

#include "bitarray.hpp"

class Huffman {
public:
    template <size_t N>
    std::unordered_map<u_int16_t, Bitarray<N>> GetCanonicalCodes(std::vector<std::pair<uint16_t, size_t>>& data) {
        auto comaparator = [](auto first, auto second) {
            if (first.second != second.second) {
                return first.second < second.second;
            }
            return first.first < second.first;
        };
        if (!std::is_sorted(data.begin(), data.end(), comaparator)) {
            std::sort(data.begin(), data.end(), comaparator);
        }
        std::unordered_map<uint16_t, Bitarray<N>> symbol_code;
        Bitarray<N> current_code(data.front().second, 0);
        symbol_code[data.front().first] = current_code;
        for (size_t i = 1; i < data.size(); ++i) {
            ++current_code;
            current_code <<= (data[i].second - data[i - 1].second);
            symbol_code[data[i].first] = current_code;
        }

        return symbol_code;
    }
};