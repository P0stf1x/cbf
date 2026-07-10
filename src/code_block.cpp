#include <variant>
#include <iostream>

#include "brainfuck.hpp"
#include "code_block.hpp"

void CodeBlock::run(size_t t) {
    for (auto& element : this->data) {
        std::visit([&t](auto& value) {
            using T = std::decay_t<decltype(value)>;

            if constexpr (std::is_same_v<T, BFOperation>) {
                std::cout << std::string(t*2, ' ') << value.c << std::endl;
            }
            else if constexpr (std::is_same_v<T, std::unique_ptr<CodeBlock>>) {
                value->run(t+1);
            }
        }, element);
    }
};
