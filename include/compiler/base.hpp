#pragma once

#include <string>

#include "code_block.hpp"

class Compiler {
    protected:
        std::string assembly;

        void compile_block(CodeBlock* starting_block);

    public:
        void compile(CodeBlock* starting_block) {
            this->compile_block(starting_block);
        };

        std::string get_asm() {
            return this->assembly;
        };
};
