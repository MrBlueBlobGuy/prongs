#include "./tokenization.h"

Tokenizer::Tokenizer(const std::string& src) {
    m_src = src;
}

std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> tokens;

    std::string buf;

    while(peek().has_value()){
        if(std::isalpha(peek().value())){
            buf.push_back(consume());
            while (peek().has_value() && std::isalnum(peek().value())){
                buf.push_back(consume());
            }
            if(buf == "exit"){
                tokens.push_back({.type = TokenType::_exit});
                buf.clear();
                continue;
            }
            else{
                std::cerr << "Something Went Wrong\n";
                std::exit(EXIT_FAILURE);
            }
        }
        else if(std::isdigit(peek().value())){
            while(peek().has_value() && std::isdigit(peek().value())){
                buf.push_back(consume());
            }
            tokens.push_back({.type = TokenType::_int_literal, .value = buf});
            buf.clear();
        }
        else if(peek().value() == ';'){
            consume();
            tokens.push_back({.type = TokenType::_semi});
            continue;
        }
        else if(std::isspace(peek().value())){
            consume();
            continue;
        }
        else{
            std::cerr << "Something Went Wrong\n";
            std::exit(EXIT_FAILURE);
        }
    }

    m_index = 0;

    return tokens;
}

std::optional<char> Tokenizer::peek(int n) {
    if(m_index+n > m_src.length()){
        return {};
    }
    else{
        return m_src.at(m_index);
    }
}

char Tokenizer::consume() {
    return m_src.at(m_index++);
}