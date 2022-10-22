#pragma once

#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

class CLIParser {
private:
    using ValueType = std::string_view;
    using SizeType = size_t;

    class Argument {
    public:
        operator bool() const;

    private:
        friend CLIParser;

        Argument(CLIParser* parser, SizeType id);

        CLIParser* parser_;
        SizeType id_;
    };

public:
    CLIParser() = default;

    const Argument AddArgument(ValueType key, ValueType description,
                               SizeType min_args = std::numeric_limits<SizeType>::min(),
                               SizeType max_args = std::numeric_limits<size_t>::max());

    const std::vector<ValueType> Parse(int& argc, char** argv);

    const std::string GetData() const;

private:
    struct CommandData {
        ValueType key;
        ValueType description;
        SizeType min_args;
        SizeType max_args;
        bool is_parsed;
    };

    std::unordered_map<ValueType, SizeType> key_to_id_;
    std::vector<CommandData> data_;
};