#pragma once

#include <compare>
#include <memory>
#include <unordered_map>
#include <vector>

class EncoderTrie {
private:
    struct Node {
        Node();

        Node(std::unique_ptr<Node>&& left, std::unique_ptr<Node>&& right);

        Node(uint16_t symbol, size_t frequency)
            : left_(nullptr), right_(nullptr), min_symbol_(symbol), frequency_(frequency) {
        }

        std::unique_ptr<Node> left_;
        std::unique_ptr<Node> right_;
        uint16_t min_symbol_;
        size_t frequency_;
    };

public:
    void BuildTrie(const std::unordered_map<uint16_t, size_t>& data);

    std::vector<std::pair<uint16_t, size_t>> GetLengths();

private:
    friend bool operator==(std::unique_ptr<Node>& first, std::unique_ptr<Node>& second);
    friend std::strong_ordering operator<=>(std::unique_ptr<Node>& first, std::unique_ptr<Node>& second);

    std::unique_ptr<Node> trie_head_;

    void FillLenghts(const Node* node, std::vector<std::pair<uint16_t, size_t>>& storage);
};