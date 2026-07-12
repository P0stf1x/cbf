#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>

#include "code_block.hpp"
#include "parser.hpp"
#include "runner.hpp"
#include "memory.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Wrong argc length. expected 1 argument: file path" << std::endl;
        exit(-1);
    }

    std::ifstream file(argv[1]);

    if (!file) {
        std::cerr << "Failed to open file" << std::endl;
        exit(-1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string input = buffer.str();

    BFParser parser(input);
    CodeBlock cb = parser.parse();

    BFMemory mem;
    BFRunner runner;
    runner.run(&cb, &mem);

    exit(0);
}
