#pragma once

#include <cstddef>
#include <exception>
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
    BitRead(std::istream& stream, const SizeType size);

    bool IsFinished() const;

    ValueType Get();

private:
    std::istream* stream_;
    const SizeType binary_size_;

    ValueType current_symbol_;
    SizeType current_symbol_bits_;

    void AssignBits(ValueType& result, SizeType& result_bits);
};

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
    BitWrite(std::ostream& stream, const SizeType size);

    void Put(ValueType value);

    void Flush();

private:
    std::ostream* stream_;
    const SizeType binary_size_;

    UCharType current_symbol_;
    SizeType current_symbol_bits_;
};