#pragma once

#include <string>
#include <stddef.h>

#include "code_block.hpp"

class BFParser {
    private:
        std::string input;

        CodeBlock parse_current_block(size_t starting_i, bool outer_block);

    public:
        BFParser(std::string input);

        CodeBlock parse();
};
