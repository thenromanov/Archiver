#pragma once

#include <cstddef>
#include <exception>
#include <iostream>

class BitRead {
public:
    using IntType = std::basic_istream<char>::int_type;
    using CharType = std::basic_istream<char>::char_type;
    using SizeType = size_t;

private:
    const SizeType int_type_size_ = sizeof(IntType) * 8;
    const SizeType char_type_size_ = sizeof(CharType) * 8;

public:
    BitRead(std::istream& stream, const SizeType size);

    bool IsFinished() const;

    IntType ReadNext();

public:
    std::istream& stream_;
    const SizeType required_bits_;

    IntType current_symbol_;
    SizeType current_symbol_bits_;

    void AssignBits(IntType& result, SizeType& result_bits);
};