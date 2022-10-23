#include "decoder.hpp"

Decoder::TrieNode::TrieNode() : left_(nullptr), right_(nullptr), symbol_(Decoder::EMPTY_SYMBOL) {
}

Decoder::TrieNode::TrieNode(uint16_t symbol) : left_(nullptr), right_(nullptr), symbol_(symbol) {
}

Decoder::Decoder() : trie_head_(std::make_unique<TrieNode>(EMPTY_SYMBOL)), automaton_(trie_head_.get()) {
}

uint16_t Decoder::AutomatonStep(bool side) {
    if (side == 0) {
        automaton_ = automaton_->left_.get();
    } else {
        automaton_ = automaton_->right_.get();
    }
    if (automaton_ == nullptr) {
        throw std::invalid_argument("Can't access this state");
    }
    uint16_t result = automaton_->symbol_;
    if (result != Decoder::EMPTY_SYMBOL) {
        automaton_ = trie_head_.get();
    }
    return result;
}