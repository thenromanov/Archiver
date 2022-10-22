#include "encoder_trie.hpp"

#include <algorithm>

#include "priority_queue.hpp"

EncoderTrie::Node::Node() : left_(nullptr), right_(nullptr), min_symbol_(0), frequency_(0) {
}

EncoderTrie::Node::Node(std::unique_ptr<Node>&& left, std::unique_ptr<Node>&& right)
    : left_(std::move(left)), right_(std::move(right)) {
    min_symbol_ = std::min(left_->min_symbol_, right_->min_symbol_);
    frequency_ = left_->frequency_ = right_->frequency_;
}

void EncoderTrie::BuildTrie(const std::unordered_map<uint16_t, size_t>& data) {
    PriorityQueue<std::unique_ptr<Node>> priority_queue;
    priority_queue.Reserve(data.size());
    for (auto [symbol, frequency] : data) {
        priority_queue.Push(std::make_unique<Node>(symbol, frequency));
    }
    while (priority_queue.Size() > 1) {
        std::unique_ptr<Node> parent =
            std::make_unique<Node>(std::move(priority_queue.Extract()), std::move(priority_queue.Extract()));
        priority_queue.Push(std::move(parent));
    }
    trie_head_ = std::move(priority_queue.Extract());
}

std::vector<std::pair<uint16_t, size_t>> EncoderTrie::GetLengths() {
    std::vector<std::pair<uint16_t, size_t>> result;
    FillLenghts(trie_head_.get(), result);
    return result;
}

bool operator==(std::unique_ptr<EncoderTrie::Node>& first, std::unique_ptr<EncoderTrie::Node>& second) {
    return (first->min_symbol_ == second->min_symbol_) && (first->frequency_ == second->frequency_);
}

std::strong_ordering operator<=>(std::unique_ptr<EncoderTrie::Node>& first,
                                 std::unique_ptr<EncoderTrie::Node>& second) {
    if (first->frequency_ != second->frequency_) {
        return first->frequency_ <=> second->frequency_;
    }
    return first->min_symbol_ <=> second->min_symbol_;
}

void EncoderTrie::FillLenghts(const Node* node, std::vector<std::pair<uint16_t, size_t>>& storage) {
    if (node->left_ == nullptr && node->right_ == nullptr) {
        storage.push_back({node->min_symbol_, node->frequency_});
        return;
    }
    if (node->left_) {
        FillLenghts(node->left_.get(), storage);
    }
    if (node->right_) {
        FillLenghts(node->right_.get(), storage);
    }
}