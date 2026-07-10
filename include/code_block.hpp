#pragma once

#include <variant>
#include <vector>

#include "brainfuck.hpp"

class CodeBlock;

using BlockItem = std::variant<
    BFOperation,
    std::unique_ptr<CodeBlock>
>;

class CodeBlock {
    private:
        std::vector<BlockItem> data;

    public:
        CodeBlock(std::vector<BlockItem> data)
            : data(std::move(data)) {};
        void run(size_t t);
};
