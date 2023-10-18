// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module lexer.source_pos;

import std.core;

export namespace lexer {

struct SourcePos
{
    SourcePos() : pos(-1), line(0), col(0) {}
    SourcePos(int line_, int col_) : pos(-1), line(line_), col(col_) {}
    SourcePos(int64_t pos_, int line_, int col_) : pos(pos_), line(line_), col(col_) {}
    bool IsValid() const { return line != 0; }
    int64_t pos;
    int line;
    int col;
};

inline bool operator==(const SourcePos& left, const SourcePos& right)
{
    return left.line == right.line && left.col == right.col;
}

inline bool operator<(const SourcePos& left, const SourcePos& right)
{
    if (left.line < right.line) return true;
    if (left.line > right.line) return false;
    return left.col < right.col;
}

inline bool operator!=(const SourcePos& left, const SourcePos& right)
{
    return !(left == right);
}

inline bool operator>(const SourcePos& left, const SourcePos& right)
{
    return right < left;
}

inline bool operator<=(const SourcePos& left, const SourcePos& right)
{
    return !(left > right);
}

inline bool operator>=(const SourcePos& left, const SourcePos& right)
{
    return !(left < right);
}

std::string ToString(const SourcePos& sourcePos);

} // namespace lexer
