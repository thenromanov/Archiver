#include "cli_parser.hpp"

#include <exception>

CLIParser::Argument::operator bool() const {
    return parser_->data_[id_].is_parsed;
}

CLIParser::Argument::Argument(CLIParser* parser, SizeType id) : parser_(parser), id_(id) {
}

const CLIParser::Argument CLIParser::AddArgument(ValueType key, ValueType description, SizeType min_args,
                                                 SizeType max_args) {
    if (key_to_id_.find(key) != key_to_id_.end()) {
        throw std::invalid_argument("Some of the arguments are already provided");
    }
    SizeType id = data_.size();
    key_to_id_[key] = id;
    data_.push_back(
        {.key = key, .description = description, .min_args = min_args, .max_args = max_args, .is_parsed = false});
    return Argument(this, id);
}

const std::vector<CLIParser::ValueType> CLIParser::Parse(int& argc, char** argv) {
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
    data_[id_it->second].is_parsed = true;
    std::vector<ValueType> arguments;
    for (int i = 2; i < argc; ++i) {
        arguments.push_back(std::string_view(argv[i]));
    }
    return arguments;
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