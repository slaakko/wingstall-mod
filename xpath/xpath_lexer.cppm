// this file has been automatically generated from 'D:/work/soulng-project/sngxml/xpath/XPathLexer.lexer' using soulng lexer generator slg version 3.0.0

export module xpath.xpath_lexer;

import lexer;
import std.core;

export class XPathLexer : public lexer::Lexer
{
public:
    XPathLexer(const std::u32string& content_, const std::string& fileName_, int fileIndex_);
    XPathLexer(const char32_t* start_, const char32_t* end_, const std::string& fileName_, int fileIndex_);
    int NextState(int state, char32_t c) override;
private:
    int GetTokenId(int statementIndex);
};
