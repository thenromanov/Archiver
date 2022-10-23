#include "bitread.hpp"

#include <exception>

BitRead::BitRead(std::istream& stream) : current_symbol_(0), current_symbol_bits_(char_type_size_) {
    if (stream.bad()) {
        throw std::invalid_argument("Given stream is bad");
    }
    stream_ = &stream;
    current_symbol_ = stream_->get();
}

void BitRead::ChangeStream(std::istream& stream) {
    if (stream.bad()) {
        throw std::invalid_argument("Given stream is bad");
    }
    stream_ = &stream;
    current_symbol_ = stream_->get();
    current_symbol_bits_ = char_type_size_;
}

bool BitRead::IsFinished() const {
    return stream_->eof();
}

BitRead::ValueType BitRead::Get(SizeType size) {
    if (IsFinished()) {
        throw std::out_of_range("All values from the stream are already read");
    }
    if (size == 0) {
        throw std::invalid_argument("Impossible to make empty buffer");
    }
    if (value_type_size_ < size) {
        throw std::invalid_argument("Given size is greater than available bits amount");
    }
    ValueType result = 0;
    AssignBits(result, size);

    while (!IsFinished() && 0 < size) {
        current_symbol_ = stream_->get();
        current_symbol_bits_ = char_type_size_;
        if (IsFinished()) {
            break;
        }
        AssignBits(result, size);
    }

    if (current_symbol_bits_ == 0) {
        current_symbol_ = stream_->get();
        current_symbol_bits_ = char_type_size_;
    }
    return result;
}

void BitRead::AssignBits(ValueType& result, SizeType& result_bits) {
    while (0 < result_bits && 0 < current_symbol_bits_) {
        result |= (((current_symbol_ >> (current_symbol_bits_ - 1)) & 1) << (result_bits - 1));
        --result_bits;
        --current_symbol_bits_;
    }
}