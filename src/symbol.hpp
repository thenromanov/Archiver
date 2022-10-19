#pragma once

#include <cinttypes>
#include <compare>
#include <cstddef>

extern const uint16_t FILENAME_END;
extern const uint16_t ONE_MORE_FILE;
extern const uint16_t ARCHIVE_END;

struct Symbol {
public:
    uint16_t symbol = 0;
    size_t count = 0;

    friend bool operator==(const Symbol& first, const Symbol& second);
    friend bool operator!=(const Symbol& first, const Symbol& second);
    friend std::strong_ordering operator<=>(const Symbol& first, const Symbol& second);
};