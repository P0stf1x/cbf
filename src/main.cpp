#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <memory>

#include "code_block.hpp"
#include "parser.hpp"
#include "runner.hpp"
#include "memory.hpp"
#include "compiler/base.hpp"
#include "compiler/aarch64.hpp"

enum class CBFMode {
    INTERPRET,
    COMPILE,
};

void print_help() {
    std::cout <<
        "Usage:\n"
        "    cbf [OPTION] <source.bf>\n"
        "\n"
        "Options:\n"
        "    -i, --interpret    Interpret Brainfuck source (default).\n"
        "    -c, --compile      Generate assembly to stdout.\n"
        "    -h, --help         Show this help message.\n";
};

int main(int argc, char** argv) {
    enum CBFMode mode = CBFMode::INTERPRET;
    if (argc < 2) {
        std::cerr << "Expected at least 1 argument" << std::endl;
        print_help();
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-i" || std::string(argv[i]) == "--interpret") { mode = CBFMode::INTERPRET; };
        if (std::string(argv[i]) == "-c" || std::string(argv[i]) == "--compile") { mode = CBFMode::COMPILE; };
        if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help") {
            print_help();
            exit(0);
        };
    }

    std::ifstream file(argv[argc-1]);

    if (!file) {
        std::cerr << "Failed to open file" << std::endl;
        exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string input = buffer.str();

    BFParser parser(input);
    CodeBlock cb = parser.parse();

    switch (mode) {
        case CBFMode::INTERPRET: {
            BFMemory mem;
            BFRunner runner;
            runner.run(&cb, &mem);
            break;
        }
        case CBFMode::COMPILE: {
            std::unique_ptr<Compiler> compiler;
            #if defined(__aarch64__) || defined(_M_ARM64)
                compiler = std::make_unique<AArch64Compiler>();
            #elif defined(__x86_64__) || defined(_M_X64)
                // TODO: make x86_64 compiler
                std::cerr << "x86_64 compiler not implemented yet";
                exit(-1);
            #else
                std::cerr << "Unknown CPU architecture";
                exit(4);
            #endif
            compiler->compile(&cb);
            std::cout << compiler->get_asm();
            break;
        }
    };


    exit(0);
}
