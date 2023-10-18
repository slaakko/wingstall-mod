// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module lexer.token;

import lexer.lexeme;
import std.core;

export namespace lexer {

const int CONTINUE_TOKEN = -2;
const int INVALID_TOKEN = -1;
const int END_TOKEN = 0;

struct Token
{
    Token() : id(INVALID_TOKEN), match(), line(1) { }
    Token(int id_) : id(id_), match(), line(1) { }
    Token(int id_, const Lexeme& match_, int line_) : id(id_), match(match_), line(line_) { }
    int id;
    Lexeme match;
    int line;
};

bool NoWhiteSpaceBetweenTokens(const Token& first, const Token& second);

std::string GetEndTokenInfo();

struct TokenLine
{
    TokenLine() : startState(0), endState(0) {}
    int TokenIndex(int columnNumber) const;
    std::vector<Token> tokens;
    int startState;
    int endState;
};

} // namespace lexer
