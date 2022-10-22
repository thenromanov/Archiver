#include "cli_parser.hpp"

#include <exception>

const CLIParser::SizeType CLIParser::AddArgument(ValueType key, ValueType description,
                                                 SizeType min_args = std::numeric_limits<SizeType>::min(),
                                                 SizeType max_args = std::numeric_limits<size_t>::max()) {
    if (key_to_id_.find(key) != key_to_id_.end()) {
        throw std::invalid_argument("Some of the arguments are already provided");
    }
    SizeType id = data_.size();
    key_to_id_[key] = id;
    data_.push_back({.key = key, .description = description, .min_args = min_args, .max_args = max_args});
    return id;
}

const std::pair<CLIParser::SizeType, std::vector<CLIParser::ValueType>> CLIParser::Parse(int& argc, char** argv) {
    ValueType last_command;
    if (argc == 1) {
        throw std::invalid_argument("No arguments provided. Use -h to get more information");
    }
    ValueType key = argv[1];
    auto id_it = key_to_id_.find(key);
    if (id_it == key_to_id_.end()) {
        throw std::invalid_argument("Invalid argument. Use -h to get more information");
    }
    if (argc - 2 < data_[id_it->second].min_args) {
        throw std::invalid_argument("Not enougth arguments. Use -h to get more information");
    } else if (argc - 2 > data_[id_it->second].max_args) {
        throw std::invalid_argument("Too many arguments. Use -h to get more information");
    }
    std::vector<ValueType> arguments;
    for (int i = 2; i < argc; ++i) {
        arguments.push_back(std::string_view(argv[i]));
    }
    return {id_it->second, arguments};
}

const std::string CLIParser::GetData() const {
    std::string result = "Usage:\n";
    for (auto command_data : data_) {
        result += '\t';
        result += command_data.key;
        result += '\t';
        result += command_data.description;
        result += '\n';
    }
    return result;
}