// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module lexer.parsing_log;

import util;
import std.core;

export namespace lexer {

class ParsingLog
{
public:
    ParsingLog();
    ParsingLog(int maxLineLength_);
    virtual ~ParsingLog();
    virtual void IncIndent() = 0;
    virtual void DecIndent() = 0;
    virtual void WriteBeginRule(const std::u32string& ruleName) = 0;
    virtual void WriteEndRule(const std::u32string& ruleName) = 0;
    virtual void WriteTry(const std::u32string& s) = 0;
    virtual void WriteSuccess(const std::u32string& match) = 0;
    virtual void WriteFail() = 0;
    virtual int MaxLineLength() const;
private:
    int maxLineLength;
};

} // namespace lexer
