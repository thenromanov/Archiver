#pragma once

#include <cstddef>
#include <exception>
#include <functional>
#include <vector>

template <typename T, typename Container = std::vector<T>, typename Compare = std::less<typename Container::value_type>>
class PriorityQueue {
private:
    using SizeType = size_t;

public:
    PriorityQueue() = default;

    PriorityQueue(std::initializer_list<T> list) {
        nodes_ = list;
        Build();
    }

    explicit PriorityQueue(const std::vector<T>& list) {
        nodes_ = list;
        Build();
    }

    explicit PriorityQueue(std::vector<T>&& list) {
        std::swap(nodes_, list);
        Build();
    }

    const T& Top() const {
        if (Empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return nodes_.front();
    }

    T Extract() {
        if (Empty()) {
            throw std::out_of_range("Queue is empty");
        }
        T value = std::move(nodes_.front());
        Pop();
        return value;
    }

    void Push(const T& value) {
        nodes_.push_back(value);
        SiftUp(nodes_.size() - 1);
    }

    void Push(T&& value) {  // value is an lvalue (it has a name), so we need to move it
        nodes_.push_back(std::move(value));
        SiftUp(nodes_.size() - 1);
    }

    void Pop() {
        if (Empty()) {
            throw std::out_of_range("Queue is empty");
        }
        std::swap(nodes_.front(), nodes_.back());
        nodes_.pop_back();
        if (!Empty()) {
            SiftDown(0);
        }
    }

    bool Empty() const {
        return nodes_.empty();
    }

    SizeType Size() const {
        return nodes_.size();
    }

    void Reserve(const SizeType size) {
        nodes_.reserve(size);
    }

private:
    std::vector<T> nodes_;

    void SiftUp(SizeType node, Compare compare = Compare()) {
        while (node > 0 && compare(nodes_[node], nodes_[(node - 1) >> 1])) {
            std::swap(nodes_[(node - 1) >> 1], nodes_[node]);
            node = (node - 1) >> 1;
        }
    }

    void SiftDown(SizeType node, Compare compare = Compare()) {
        while (node * 2 + 1 < nodes_.size()) {
            const SizeType left_node = node * 2 + 1;
            const SizeType right_node = node * 2 + 2;
            const SizeType next_node =
                (right_node < nodes_.size() && compare(nodes_[right_node], nodes_[left_node]) ? right_node : left_node);
            if (compare(nodes_[node], nodes_[next_node])) {
                break;
            }
            std::swap(nodes_[node], nodes_[next_node]);
            node = next_node;
        }
    }

    void Build() {  // O(n) build
        for (SizeType i = nodes_.size(); i > 0; --i) {
            SiftDown(i - 1);
        }
    }
};