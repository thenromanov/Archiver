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

    Bitarray(SizeType value, SizeType bits_count) : storage_(value), bits_count_(bits_count) {
    }

    explicit Bitarray(SizeType value) : Bitarray(value, 1) {
        if (value == 0) {
            return;
        }
        bits_count_ = 0;
        while (value > 0) {
            value <<= 1;
            ++bits_count_;
        }
    }

    Bitarray(const Bitarray& other) {
        storage_ = other.storage_;
        bits_count_ = other.bits_count_;
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
        std::swap(bits_count_, other.bits_count_);
    }
};