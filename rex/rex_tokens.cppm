// this file has been automatically generated from 'D:/work/soulng-project/soulng/rex/RexLexer.lexer' using soulng lexer generator slg version 3.0.0

export module rex.tokens;

import std.core;

export namespace RexTokens
{
    const int END = 0;
    const int LPAREN = 1;
    const int RPAREN = 2;
    const int LBRACKET = 3;
    const int RBRACKET = 4;
    const int ALT = 5;
    const int STAR = 6;
    const int PLUS = 7;
    const int QUEST = 8;
    const int DOT = 9;
    const int ESCAPE = 10;
    const int INVERSE = 11;
    const int MINUS = 12;
    const int CHAR = 13;

    void InitTokenIdMap();
    int GetTokenId(const std::u32string& tokenName);
    const char* GetTokenName(int tokenId);
    const char* GetTokenInfo(int tokenId);
}
