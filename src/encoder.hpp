#pragma once

#include <algorithm>
#include <compare>
#include <memory>
#include <unordered_map>
#include <vector>

#include "bitarray.hpp"

class Encoder {
private:
    struct TrieNode {
        TrieNode();

        TrieNode(std::unique_ptr<TrieNode>&& left, std::unique_ptr<TrieNode>&& right);

        TrieNode(uint16_t symbol, size_t frequency);

        std::unique_ptr<TrieNode> left_;
        std::unique_ptr<TrieNode> right_;
        uint16_t min_symbol_;
        size_t frequency_;
    };

    struct TrieNodeCompare {
        bool operator()(const std::unique_ptr<TrieNode>& first, const std::unique_ptr<TrieNode>& second) const {
            if (first->frequency_ == second->frequency_) {
                return first->min_symbol_ < second->min_symbol_;
            }
            return first->frequency_ < second->frequency_;
        }
    };

public:
    std::vector<std::pair<uint16_t, size_t>> GetCodesLengths(const std::unordered_map<uint16_t, size_t>& data);

    template <size_t N>
    std::unordered_map<u_int16_t, Bitarray<N>> GetCanonicalCodes(const std::vector<std::pair<uint16_t, size_t>>& data) {
        if (!std::is_sorted(data.begin(), data.end(), [](auto first, auto second) {
                if (first.second != second.second) {
                    return first.second < second.second;
                }
                return first.first < second.first;
            })) {
            throw std::invalid_argument("Data must be sorted");
        }
        std::unordered_map<uint16_t, Bitarray<N>> symbol_code;
        Bitarray<N> current_code(0, data.front().second);
        symbol_code[data.front().first] = current_code;
        for (size_t i = 1; i < data.size(); ++i) {
            ++current_code;
            current_code <<= (data[i].second - data[i - 1].second);
            symbol_code[data[i].first] = current_code;
        }

        return symbol_code;
    }

private:
    std::unique_ptr<TrieNode> trie_head_;

    void BuildTrie(const std::unordered_map<uint16_t, size_t>& data);
    void FillLenghts(const TrieNode* TrieNode, std::vector<std::pair<uint16_t, size_t>>& storage, size_t depth = 0);
};