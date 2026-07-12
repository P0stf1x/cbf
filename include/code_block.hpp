#pragma once

#include <variant>
#include <vector>
#include <memory>

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

        using iterator = std::vector<BlockItem>::iterator;
        using const_iterator = std::vector<BlockItem>::const_iterator;


        iterator begin() { return this->data.begin(); };
        iterator end() { return this->data.end(); };

        const_iterator begin() const { return this->data.begin(); };
        const_iterator end() const { return this->data.end(); };

        size_t size() const { return this->data.size(); }
        bool empty() const { return this->data.empty(); }
};
