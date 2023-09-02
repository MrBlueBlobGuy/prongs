#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <optional>
#include <vector>

enum class TokenType{
    _exit,
    _int_literal,
    _semi
};

struct Token{
    TokenType type;
    std::optional<std::string> value {};
};

std::vector<Token> tokenize(const std::string& str){
    std::vector<Token> tokens;

    std::string buf;
    for(int i = 0; i<str.length(); i++){
        char c = str.at(i);
        if(std::isalpha(c)){
            buf.push_back(c);
            i++;
            while(std::isalnum(str.at(i))){
                buf.push_back(str.at(i));
                i++;
            }
            i--;

            if(buf == "exit") {
                tokens.push_back({.type = TokenType::_exit});
                buf.clear();
                continue;
            }
            else{
                std::cerr << "Something Went Wrong\n";
                std::exit(EXIT_FAILURE);
            }
        }
        else if(std::isspace(c)){
            continue;
        }
        else if(std::isdigit(c)){
            buf.push_back(c);
            i++;
            while(std::isdigit(str.at(i))){
                buf.push_back(str.at(i));
                i++;
            }
            i--;
            tokens.push_back({.type = TokenType::_int_literal, .value = buf});
        }
        else if (c==';'){
            tokens.push_back({.type = TokenType::_semi});
        }
        else{
            std::cerr << "Something Went Wrong\n";
            std::exit(EXIT_FAILURE);
        }
    }
    return tokens;
}

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
    return output.str();
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

    std::vector<Token> tokens = tokenize(contents);


    {
        std::fstream file("out.asm", std::ios::out);
        file << tokens_to_asm(tokens) << "\n";
    }

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");

    return EXIT_SUCCESS;
}