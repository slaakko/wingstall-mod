// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module lexer.keyword;

import lexer.lexeme;
import lexer.token;
import std.core;

export namespace lexer {

struct Keyword
{
    Keyword() : str(nullptr), tokenID(INVALID_TOKEN) {}
    Keyword(const char32_t* str_, int tokenID_) : str(str_), tokenID(tokenID_) { }
    const char32_t* str;
    int tokenID;
};

class KeywordMap
{
public:
    KeywordMap(const Keyword* keywords_);
    int GetKeywordToken(const Lexeme& lexeme) const;
private:
    const Keyword* keywords;
    std::map<Lexeme, int> keywordMap;
};

} // namespace lexer
