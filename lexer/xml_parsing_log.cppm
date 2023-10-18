// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module lexer.xml_parsing_log;

import lexer.parsing_log;
import util;
import std.core;

export namespace lexer {

class XmlParsingLog : public ParsingLog
{
public:
    XmlParsingLog(std::ostream& stream_);
    XmlParsingLog(std::ostream& stream_, int maxLineLength_);
    void IncIndent() override;
    void DecIndent() override;
    void WriteBeginRule(const std::u32string& ruleName) override;
    void WriteEndRule(const std::u32string& ruleName) override;
    void WriteTry(const std::u32string& s) override;
    void WriteSuccess(const std::u32string& match) override;
    void WriteFail() override;
    void WriteElement(const std::u32string& elementName, const std::u32string& elementContent);
    void Write(const std::u32string& s);
    util::CodeFormatter& Formatter() { return formatter; }
private:
    util::CodeFormatter formatter;
};

std::u32string XmlHexEscape(char32_t c);
std::u32string XmlCharStr(char32_t c);
std::u32string XmlEscape(const std::u32string& s);

} // namespace lexer
