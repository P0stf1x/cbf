#include <vector>
#include <string>
#include <format>
#include <iostream>

#include "compiler/aarch64.hpp"
#include "code_block.hpp"

void AArch64Compiler::compile_block(CodeBlock* cb) {
    #if defined(__APPLE__)
        std::string main_sym = "_main";
        std::string putchar_sym = "_putchar";
        std::string getchar_sym = "_getchar";
        std::string malloc_sym = "_malloc";
        std::string free_sym = "_free";
    #else
        std::string main_sym = "main";
        std::string putchar_sym = "putchar";
        std::string getchar_sym = "getchar";
        std::string malloc_sym = "malloc";
        std::string free_sym = "free";
    #endif
    // constexpr auto header_str =;
    std::string header = std::format(
        ".text\n"
        ".global {0}\n"
        "{0}:\n"
        "init_bf:\n"
        "stp x19, x20, [sp, #-16]! // x19 = memory pointer, x20 = head\n"
        "stp x29, x30, [sp, #-16]!\n"
        "mov x20, #0\n",
        main_sym
    );
    std::string allocate_memory = std::format(
        "allocate_memory:\n"
        "mov x0, #32768\n"
        "bl {0}\n"
        "mov x19, x0\n",
        malloc_sym
    );
    std::string deallocate_memory = std::format(
        "deallocate_memory:\n"
        "mov x0, x19\n"
        "bl {0}\n",
        free_sym
    );
    std::string finish =
        "deinit:\n"
        "ldp x29, x30, [sp], #16\n"
        "ldp x19, x20, [sp], #16\n"
        "exit_bf:\n"
        "mov x0, #0\n"
        "ret\n";
    std::string output_data = std::format(
        "stp x29, x30, [sp, #-16]!\n"
        "ldr x0, [x19, x20]\n"
        "and x0, x0, #0xFF\n"
        "bl {}\n"
        "ldp x29, x30, [sp], #16\n",
        putchar_sym
    );
    std::string input_data = std::format(
        "stp x29, x30, [sp, #-16]!\n"
        "bl {}\n"
        "str x0, [x19, x20]\n"
        "ldp x29, x30, [sp], #16\n",
        getchar_sym
    );
    std::string inc_data =
        "ldr x0, [x19, x20]\n"
        "add x0, x0, #1\n"
        "str x0, [x19, x20]\n";
    std::string dec_data =
        "ldr x0, [x19, x20]\n"
        "sub x0, x0, #1\n"
        "str x0, [x19, x20]\n";
    std::string inc_head =
        "add x20, x20, #1\n"
        "and x20, x20, #0x7FFF\n";
    std::string dec_head =
        "sub x20, x20, #1\n"
        "and x20, x20, #0x7FFF\n";

    this->assembly = header + allocate_memory + "bf_program:\n";
    for (auto& element : *cb) {
        std::visit([this, inc_head, dec_head, inc_data, dec_data, output_data, input_data](auto& value) {
            using T = std::decay_t<decltype(value)>;

            if constexpr (std::is_same_v<T, BFOperation>) {
                std::string asm_line;
                switch (value.type) {
                    case BFOperationType::POINTER_INCREMENT:
                        asm_line = inc_head;
                        break;
                    case BFOperationType::POINTER_DECREMENT:
                        asm_line = dec_head;
                        break;
                    case BFOperationType::DATA_INCREMENT:
                        asm_line = inc_data;
                        break;
                    case BFOperationType::DATA_DECREMENT:
                        asm_line = dec_data;
                        break;
                    case BFOperationType::OUTPUT:
                        asm_line = output_data;
                        break;
                    case BFOperationType::INPUT:
                        asm_line = input_data;
                        break;
                    default:
                        break;
                }
                this->assembly += asm_line;
            }
            else if constexpr (std::is_same_v<T, std::unique_ptr<CodeBlock>>) {
                std::cerr << "Loops not implemented in compile mode" << std::endl;
            }
        }, element);
    }
    this->assembly += deallocate_memory + finish;
};
