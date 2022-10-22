#pragma once

#include <algorithm>
#include <compare>
#include <memory>
#include <unordered_map>
#include <vector>

#include "priority_queue.hpp"

class EncoderTrie {
private:
    struct Node {
        Node() : left_(nullptr), right_(nullptr), min_symbol_(0), frequency_(0) {
        }

        Node(std::unique_ptr<Node>&& left, std::unique_ptr<Node>&& right)
            : left_(std::move(left)), right_(std::move(right)) {
            min_symbol_ = std::min(left_->min_symbol_, right_->min_symbol_);
            frequency_ = left_->frequency_ = right_->frequency_;
        }

        Node(uint16_t symbol, size_t frequency)
            : left_(nullptr), right_(nullptr), min_symbol_(symbol), frequency_(frequency) {
        }

        std::unique_ptr<Node> left_;
        std::unique_ptr<Node> right_;
        uint16_t min_symbol_;
        size_t frequency_;
    };

public:
    void BuildTrie(std::unordered_map<uint16_t, size_t>& data) {
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

    std::vector<std::pair<uint16_t, size_t>> GetLengths() {
        std::vector<std::pair<uint16_t, size_t>> result;
        FillLenghts(trie_head_.get(), result);
        return result;
    }

private:
    friend bool operator==(std::unique_ptr<Node>& first, std::unique_ptr<Node>& second);
    friend std::strong_ordering operator<=>(std::unique_ptr<Node>& first, std::unique_ptr<Node>& second);

    std::unique_ptr<Node> trie_head_;

    void FillLenghts(const Node* node, std::vector<std::pair<uint16_t, size_t>>& storage) {
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
};