#include <variant>
#include <vector>
#include <iostream>

#include "parser.hpp"
#include "code_block.hpp"
#include "brainfuck.hpp"

BFParser::BFParser(std::string input)
    : input(input) {};

CodeBlock BFParser::parse_current_block(size_t starting_i, bool outer_block) {
    std::vector<BlockItem> result;
    size_t i = starting_i;
    ssize_t depth = outer_block ? 0 : 0;
    while (i < this->input.length()) {

        if (this->input[i] == '[') {
            depth++;
            if (depth == 1) {
                result.emplace_back(
                    std::make_unique<CodeBlock>(
                        this->parse_current_block((i+1), false)
                    )
                );
            }
        }

        if (this->input[i] == ']') {
            depth--;
            if (depth == -1) {
                if (!outer_block) {
                    break;
                } else {
                    std::cerr << "Closed non existant loop at symbol " << i << std::endl;
                    exit(-3);
                }
            }
        }

        if (depth == 0) {
            switch (this->input[i]) {
                case '>':
                    result.emplace_back(BFOperation{
                        .type = BFOperationType::POINTER_INCREMENT,
                        .count = 1,
                    });
                    break;
                case '<':
                    result.emplace_back(BFOperation{
                        .type = BFOperationType::POINTER_DECREMENT,
                        .count = 1,
                    });
                    break;
                case '+':
                    result.emplace_back(BFOperation{
                        .type = BFOperationType::DATA_INCREMENT,
                        .count = 1,
                    });
                    break;
                case '-':
                    result.emplace_back(BFOperation{
                        .type = BFOperationType::DATA_DECREMENT,
                        .count = 1,
                    });
                    break;
                case '.':
                    result.emplace_back(BFOperation{
                        .type = BFOperationType::OUTPUT,
                        .count = 1,
                    });
                    break;
                case ',':
                    result.emplace_back(BFOperation{
                        .type = BFOperationType::INPUT,
                        .count = 1,
                    });
                    break;
            }
        }

        i++;
    }

    if (depth > 0) {
        std::cerr << "Loop at symbol " << starting_i << " not closed" << std::endl;
        std::cerr << "FIXME: loop started not at 0" << std::endl; // FIXME: uhhh this runs on outer_block, so starting_i is 0
        exit(-2);
    }

    return CodeBlock(std::move(result));
};

CodeBlock BFParser::parse() {
    return this->parse_current_block(0, true);
};
