#pragma once

#include "code_block.hpp"
#include "memory.hpp"

class BFRunner {
    private:
        void run_block(CodeBlock* cb, BFMemory* mem);

    public:
        void run(CodeBlock* cb, BFMemory* mem);
};
