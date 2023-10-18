// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module lexer.trivial_lexer;

import lexer.lexer;
import std.core;

export class TrivialLexer : public lexer::Lexer
{
public:
    TrivialLexer(const std::u32string& content_, const std::string& fileName_, int fileIndex_);
    TrivialLexer(const char32_t* start_, const char32_t* end_, const std::string& fileName_, int fileIndex_);
    int NextState(int state, char32_t c) override;
};

std::string GetTrivialTokenInfo(int tokenId);
