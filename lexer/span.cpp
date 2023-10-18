// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module lexer.span;

namespace lexer {

bool operator==(const Span& left, const Span& right)
{
    return left.fileIndex == right.fileIndex && left.line == right.line && left.start == right.start && left.end == right.end;
}

} // namespace lexer
