#pragma once

#include <array>
#include <cstdint>

constexpr size_t MEMORY_SIZE = 1024 * 32; // 32kb, or 32'768 bytes. Because 32kb is easier than 30kib

struct BFMemory {
    std::array<uint8_t, MEMORY_SIZE> data{};
    size_t head = 0;

    bool cell_empty() {
        return data[head] == 0;
    }
};
