// this file has been automatically generated from 'D:/work/soulng-project/soulng/rex/RexLexer.lexer' using soulng lexer generator slg version 3.0.0

export module rex.lexer;

import lexer;
import std.core;

export class RexLexer : public lexer::Lexer
{
public:
    RexLexer(const std::u32string& content_, const std::string& fileName_, int fileIndex_);
    RexLexer(const char32_t* start_, const char32_t* end_, const std::string& fileName_, int fileIndex_);
    int NextState(int state, char32_t c) override;
private:
    int GetTokenId(int statementIndex);
};
