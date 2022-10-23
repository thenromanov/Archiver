#pragma once

#include <cstddef>
#include <iostream>
#include <type_traits>

class BitRead {
public:
    using IntType = std::basic_ostream<char>::int_type;
    using CharType = std::basic_istream<char>::char_type;
    using ValueType = std::make_unsigned<IntType>::type;
    using SizeType = size_t;

private:
    const SizeType value_type_size_ = sizeof(ValueType) * 8;
    const SizeType char_type_size_ = sizeof(CharType) * 8;

public:
    BitRead(std::istream& stream);

    void ChangeStream(std::istream& stream);

    bool IsFinished() const;

    ValueType Get(SizeType size);

private:
    std::istream* stream_;

    ValueType current_symbol_;
    SizeType current_symbol_bits_;

    void AssignBits(ValueType& result, SizeType& result_bits);
};