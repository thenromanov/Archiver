#pragma once

#include <memory>

#include "huffman.hpp"

class Decoder : public Huffman {
private:
    struct TrieNode {
        TrieNode();

        explicit TrieNode(uint16_t symbol);

        std::unique_ptr<TrieNode> left_;
        std::unique_ptr<TrieNode> right_;
        uint16_t symbol_;
    };

public:
    static constexpr uint16_t EMPTY_SYMBOL = std::numeric_limits<uint16_t>::max();

    Decoder();

    template <size_t N>
    void InsertBitarry(uint16_t symbol, const Bitarray<N>& binary_code) {
        TrieNode* node = trie_head_.get();
        for (size_t i = binary_code.GetSize(); i > 0; --i) {
            if (binary_code[i - 1] == 0) {
                if (node->left_ == nullptr) {
                    node->left_ = std::make_unique<TrieNode>(EMPTY_SYMBOL);
                }
                node = node->left_.get();
            } else {
                if (node->right_ == nullptr) {
                    node->right_ = std::make_unique<TrieNode>(EMPTY_SYMBOL);
                }
                node = node->right_.get();
            }
        }
        node->symbol_ = symbol;
    }

    uint16_t AutomatonStep(bool side);

private:
    std::unique_ptr<TrieNode> trie_head_;

    TrieNode* automaton_;
};