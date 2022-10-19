#include "symbol.hpp"

const uint16_t FILENAME_END = 256;
const uint16_t ONE_MORE_FILE = 257;
const uint16_t ARCHIVE_END = 258;

bool operator==(const Symbol& first, const Symbol& second) {
    return (first.count == second.count) && (first.symbol == second.symbol);
}

bool operator!=(const Symbol& first, const Symbol& second) {
    return !(first == second);
}

std::strong_ordering operator<=>(const Symbol& first, const Symbol& second) {
    if (first.count != second.count) {
        return first.count <=> second.count;
    }
    return first.symbol <=> second.symbol;
}