// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module lexer.lexeme;

import std.core;

export namespace lexer {

struct Lexeme
{
    Lexeme() : begin(nullptr), end(nullptr) { }
    Lexeme(const char32_t* begin_, const char32_t* end_) : begin(begin_), end(end_) { }
    std::u32string ToString() const { return std::u32string(begin, end); }
    const char32_t* begin;
    const char32_t* end;
};

inline bool operator==(const Lexeme& left, const Lexeme& right)
{
    if (left.end - left.begin != right.end - right.begin) return false;
    const char32_t* p = left.begin;
    const char32_t* q = right.begin;
    while (p != left.end)
    {
        if (*p != *q) return false;
        ++p;
        ++q;
    }
    return true;
}

inline bool operator<(const Lexeme& left, const Lexeme& right)
{
    const char32_t* p = left.begin;
    const char32_t* q = right.begin;
    while (p != left.end && q != right.end)
    {
        if (*p < *q) return true;
        if (*p > *q) return false;
        ++p;
        ++q;
    }
    if (p == left.end) return q != right.end;
    return false;
}

} // namespace lexer
