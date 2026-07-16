#pragma once

#include <string>

#include "code_block.hpp"
#include "compiler/base.hpp"

class AArch64Compiler : public Compiler {
    private:
        std::string assembly;

        void compile_block(CodeBlock* cb);

    public:
        void compile(CodeBlock* starting_block) {
            this->compile_block(starting_block);
        };

        std::string get_asm() {
            return this->assembly;
        };
};
