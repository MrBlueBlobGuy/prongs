#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <vector>

#include "./tokenization.h"


std::string tokens_to_asm(std::vector<Token>& tokens){
    std::stringstream output;

    output<< "global _start \n_start: \n";

    for(int i = 0; i < tokens.size(); i++){
        const Token token = tokens.at(i);
        if(token.type == TokenType::_exit){
            if(i+1<tokens.size() && tokens.at(i+1).type == TokenType::_int_literal){
                if(i+2 < tokens.size() && tokens.at(i+2).type == TokenType::_semi){
                    output << "    mov rax, 60\n";
                    output << "    mov rdi, " << tokens.at(i+1).value.value() << "\n";
                    output << "    syscall";
                }
            }
        }
    }
    std::string opt_string = output.str();
    return opt_string;
}

int main(int argc, char* argv[]){
    if(argc != 2){
        std::cerr << "Incorrect usage correct usage is... \n";
        std::cerr << "prongs <input.pr> \n";
        return EXIT_FAILURE;
    }

    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream<<input.rdbuf();
        contents = contents_stream.str();
    }

    Tokenizer tokenizer(contents);
    std::vector<Token> tokens = tokenizer.tokenize();


    {
        std::fstream file("out.asm", std::ios::out);
        file << tokens_to_asm(tokens) << "\n";
    }

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");

    std::cout<<"\n";

    return EXIT_SUCCESS;
}