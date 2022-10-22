#include "bitwrite.hpp"

#include <exception>

BitWrite::BitWrite(std::ostream& stream) : current_symbol_(0), current_symbol_bits_(0) {
    if (stream.bad()) {
        throw std::invalid_argument("Given stream is bad");
    }
    stream_ = &stream;
}

void BitWrite::Put(ValueType value, const SizeType size) {
    if (size == 0) {
        throw std::invalid_argument("Impossible to write empty buffer");
    }
    if (int_type_size_ < size) {
        throw std::invalid_argument("Given size is greater than available bits amount");
    }
    for (SizeType bit = 0; bit < size; ++bit) {
        current_symbol_ |= ((value >> bit) & 1) << current_symbol_bits_;
        ++current_symbol_bits_;
        if (current_symbol_bits_ == char_type_size_) {
            stream_->put(static_cast<CharType>(current_symbol_));
            current_symbol_ = 0;
            current_symbol_bits_ = 0;
        }
    }
}

void BitWrite::Flush() {
    if (current_symbol_bits_ == 0) {
        return;
    }
    stream_->put(static_cast<CharType>(current_symbol_));
    current_symbol_ = 0;
    current_symbol_bits_ = 0;
}