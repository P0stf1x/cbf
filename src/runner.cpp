#include <variant>
#include <iostream>
#include <array>
#include <memory>
#include <type_traits>
#include <vector>

#include "runner.hpp"
#include "code_block.hpp"
#include "memory.hpp"
#include "brainfuck.hpp"

void BFRunner::run_block(CodeBlock* cb, BFMemory* mem) {
    for (auto& element : *cb) {
        std::visit([this, &mem](auto& value) {
            using T = std::decay_t<decltype(value)>;

            if constexpr (std::is_same_v<T, BFOperation>) {
                switch (value.type)
                {
                case BFOperationType::POINTER_INCREMENT:
                    mem->head = (mem->head+1)&0x7FFF;
                    break;
                case BFOperationType::POINTER_DECREMENT:
                    mem->head = (mem->head-1)&0x7FFF;
                    break;
                case BFOperationType::DATA_INCREMENT:
                    mem->data[mem->head]++;
                    break;
                case BFOperationType::DATA_DECREMENT:
                    mem->data[mem->head]--;
                    break;
                case BFOperationType::OUTPUT:
                    std::cout << mem->data[mem->head];
                    break;
                case BFOperationType::INPUT:
                    mem->data[mem->head] = std::cin.get();
                    break;
                default:
                    break;
                }
            }
            else if constexpr (std::is_same_v<T, std::unique_ptr<CodeBlock>>) {
                while (!mem->cell_empty()) {
                    this->run_block(value.get(), mem);
                }
            }
        }, element);
    }
};

void BFRunner::run(CodeBlock* cb, BFMemory* mem) {
    this->run_block(cb, mem);
};
