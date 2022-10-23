#pragma once

#include <algorithm>
#include <compare>
#include <memory>
#include <unordered_map>
#include <vector>

#include "huffman.hpp"

class Encoder : public Huffman {
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
        bool operator()(const std::unique_ptr<TrieNode>& first, const std::unique_ptr<TrieNode>& second) const;
    };

public:
    std::vector<std::pair<uint16_t, size_t>> GetCodesLengths(const std::unordered_map<uint16_t, size_t>& data);

private:
    std::unique_ptr<TrieNode> trie_head_;

    void BuildTrie(const std::unordered_map<uint16_t, size_t>& data);
    void FillLenghts(const TrieNode* TrieNode, std::vector<std::pair<uint16_t, size_t>>& storage, size_t depth = 0);
};