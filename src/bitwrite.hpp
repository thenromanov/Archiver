#pragma once

#include <cstddef>
#include <iostream>
#include <type_traits>

class BitWrite {
public:
    using IntType = std::basic_ostream<char>::int_type;
    using CharType = std::basic_ostream<char>::char_type;
    using ValueType = std::make_unsigned<IntType>::type;
    using UCharType = std::make_unsigned<CharType>::type;
    using SizeType = size_t;

private:
    const SizeType int_type_size_ = sizeof(IntType) * 8;
    const SizeType char_type_size_ = sizeof(CharType) * 8;

public:
    BitWrite(std::ostream& stream);

    void Put(ValueType value, const SizeType size);

    void Flush();

private:
    std::ostream* stream_;

    UCharType current_symbol_;
    SizeType current_symbol_bits_;
};