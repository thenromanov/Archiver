#include "encoder.hpp"

#include <algorithm>
#include <iostream>

#include "priority_queue.hpp"

Encoder::TrieNode::TrieNode() : left_(nullptr), right_(nullptr), min_symbol_(0), frequency_(0) {
}

Encoder::TrieNode::TrieNode(std::unique_ptr<TrieNode>&& left, std::unique_ptr<TrieNode>&& right)
    : left_(std::move(left)), right_(std::move(right)) {
    min_symbol_ = std::min(left_->min_symbol_, right_->min_symbol_);
    frequency_ = left_->frequency_ + right_->frequency_;
}

Encoder::TrieNode::TrieNode(uint16_t symbol, size_t frequency)
    : left_(nullptr), right_(nullptr), min_symbol_(symbol), frequency_(frequency) {
}

std::vector<std::pair<uint16_t, size_t>> Encoder::GetCodesLengths(const std::unordered_map<uint16_t, size_t>& data) {
    BuildTrie(data);
    std::vector<std::pair<uint16_t, size_t>> result;
    FillLenghts(trie_head_.get(), result);
    return result;
}

void Encoder::BuildTrie(const std::unordered_map<uint16_t, size_t>& data) {
    PriorityQueue<std::unique_ptr<TrieNode>, std::vector<std::unique_ptr<TrieNode>>, TrieNodeCompare> priority_queue;
    priority_queue.Reserve(data.size());
    for (auto [symbol, frequency] : data) {
        priority_queue.Push(std::make_unique<TrieNode>(symbol, frequency));
    }
    while (priority_queue.Size() > 1) {
        std::unique_ptr<TrieNode> parent =
            std::make_unique<TrieNode>(priority_queue.Extract(), priority_queue.Extract());
        priority_queue.Push(std::move(parent));
    }
    trie_head_ = priority_queue.Extract();
}

void Encoder::FillLenghts(const TrieNode* node, std::vector<std::pair<uint16_t, size_t>>& storage, size_t depth) {
    if (node->left_ == nullptr && node->right_ == nullptr) {
        storage.push_back({node->min_symbol_, depth});
        return;
    }
    if (node->left_) {
        FillLenghts(node->left_.get(), storage, depth + 1);
    }
    if (node->right_) {
        FillLenghts(node->right_.get(), storage, depth + 1);
    }
}