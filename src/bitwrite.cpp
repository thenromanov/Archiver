#include "bitwrite.hpp"

#include <exception>

BitWrite::BitWrite(std::ostream& stream) : current_symbol_(0), current_symbol_bits_(char_type_size_) {
    if (stream.bad()) {
        throw std::invalid_argument("Given stream is bad");
    }
    stream_ = &stream;
}

void BitWrite::ChangeStream(std::ostream& stream) {
    if (stream.bad()) {
        throw std::invalid_argument("Given stream is bad");
    }
    stream_ = &stream;
    current_symbol_ = 0;
    current_symbol_bits_ = char_type_size_;
}

void BitWrite::Put(ValueType value, SizeType size) {
    if (size == 0) {
        throw std::invalid_argument("Impossible to write empty buffer");
    }
    if (int_type_size_ < size) {
        throw std::invalid_argument("Given size is greater than available bits amount");
    }
    while (0 < size) {
        current_symbol_ |= ((value >> (size - 1)) & 1) << (current_symbol_bits_ - 1);
        --current_symbol_bits_;
        --size;
        if (current_symbol_bits_ == 0) {
            stream_->put(static_cast<CharType>(current_symbol_));
            current_symbol_ = 0;
            current_symbol_bits_ = char_type_size_;
        }
    }
}

void BitWrite::Flush() {
    if (current_symbol_bits_ == char_type_size_) {
        return;
    }
    stream_->put(static_cast<CharType>(current_symbol_));
    current_symbol_ = 0;
    current_symbol_bits_ = char_type_size_;
}