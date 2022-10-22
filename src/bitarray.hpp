#pragma once

#include <bitset>
#include <cstddef>

template <size_t N>
class Bitarray {
private:
    using SizeType = size_t;

public:
    Bitarray() : storage_(0), bits_count_(1) {
    }

    explicit Bitarray(SizeType value) : storage_(value), bits_count_(0) {
        if (value == 0) {
            bits_count_ = 1;
            return;
        }
        while (value > 0) {
            value <<= 1;
            ++bits_count_;
        }
    }

    Bitarray(const Bitarray& other) {
        storage_ = other.storage_;
    }

    Bitarray(Bitarray&& other) {
        Swap(other);
    }

    Bitarray& operator=(const Bitarray& other) & {
        Bitarray new_arr(other);
        Swap(new_arr);
        return *this;
    }

    Bitarray& operator=(Bitarray&& other) & {
        Swap(other);
    }

    Bitarray& operator++() {
        for (SizeType i = 0; i < bits_count_; ++i) {
            if (storage_[i] == 0) {
                storage_[i] = 1;
                return *this;
            }
            storage_[i] = 0;
        }
        storage_[bits_count_] = 1;
        ++bits_count_;
        return *this;
    }

    Bitarray& operator<<=(SizeType k) {
        bits_count_ += k;
        storage_ <<= k;
        return *this;
    }

    SizeType GetSize() const {
        return bits_count_;
    }

    bool operator[](SizeType index) {
        return storage_[index];
    }

private:
    std::bitset<N> storage_;
    SizeType bits_count_;

    void Swap(Bitarray& other) {
        std::swap(storage_, other.storage_);
    }
};