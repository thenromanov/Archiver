#pragma once

#include <cstddef>
#include <exception>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

class CLIParser {
private:
    using ValueType = std::string_view;
    using SizeType = size_t;

public:
    CLIParser() = default;

    const SizeType AddArgument(ValueType key, ValueType description, SizeType min_args, SizeType max_args);

    const std::pair<SizeType, std::vector<ValueType>> Parse(int& argc, char** argv);

    const std::string GetData() const;

private:
    struct CommandData {
        ValueType key;
        ValueType description;
        SizeType min_args;
        SizeType max_args;
    };

    std::unordered_map<ValueType, SizeType> key_to_id_;
    std::vector<CommandData> data_;
};