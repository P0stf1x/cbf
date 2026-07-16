#pragma once

#include <string>

#include "code_block.hpp"
#include "compiler/base.hpp"

class AArch64Compiler : public Compiler {
    private:
        void compile_block(CodeBlock* cb) override;
};
