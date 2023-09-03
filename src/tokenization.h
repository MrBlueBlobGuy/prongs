#pragma once

#include <string>
#include <vector>
#include <optional>
#include <iostream>

enum class TokenType{
    _exit,
    _int_literal,
    _semi
};

struct Token{
    TokenType type;
    std::optional<std::string> value {};
};

class Tokenizer{
public:
    explicit Tokenizer(const std::string& src);
    std::vector<Token> tokenize();
private:
    [[nodiscard]]std::optional<char> peek(int n=1);

    char consume();

    std::string m_src;
    int m_index = 0;
};