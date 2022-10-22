#include "bitread.hpp"

#include <exception>

BitRead::BitRead(std::istream& stream, const SizeType size)
    : binary_size_(size), current_symbol_(0), current_symbol_bits_(0) {
    if (size == 0) {
        throw std::invalid_argument("Impossible to make empty buffer");
    }
    if (value_type_size_ < size) {
        throw std::invalid_argument("Given size is greater than available bits amount");
    }
    if (stream.bad()) {
        throw std::invalid_argument("Given stream is bad");
    }
    stream_ = &stream;
    current_symbol_ = stream_->get();
}

bool BitRead::IsFinished() const {
    return stream_->eof();
}

BitRead::ValueType BitRead::Get() {
    if (IsFinished()) {
        throw std::out_of_range("All values from the stream are already read");
    }

    ValueType result = 0;
    SizeType result_bits = 0;
    AssignBits(result, result_bits);

    while (!IsFinished() && result_bits < binary_size_) {
        current_symbol_ = stream_->get();
        current_symbol_bits_ = 0;
        if (IsFinished()) {
            break;
        }
        AssignBits(result, result_bits);
    }

    if (current_symbol_bits_ == char_type_size_) {
        current_symbol_ = stream_->get();
        current_symbol_bits_ = 0;
    }
    return result;
}

void BitRead::AssignBits(ValueType& result, SizeType& result_bits) {
    while (result_bits < binary_size_ && current_symbol_bits_ < char_type_size_) {
        result |= (((current_symbol_ >> current_symbol_bits_) & 1) << result_bits);
        ++result_bits;
        ++current_symbol_bits_;
    }
}