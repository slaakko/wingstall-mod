// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module lexer.parsing_log;

namespace lexer {

ParsingLog::ParsingLog() : maxLineLength(80)
{
}

ParsingLog::ParsingLog(int maxLineLength_) : maxLineLength(maxLineLength_)
{
}

ParsingLog::~ParsingLog()
{
}

int ParsingLog::MaxLineLength() const
{
    return maxLineLength;
}

} // namespace lexer
