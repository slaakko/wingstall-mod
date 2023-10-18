// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module dom.cdata_section;

import std.core;
import dom.text;

export namespace xml {

class CDataSection : public Text
{
public:
    CDataSection(const lexer::SourcePos& sourcePos_);
    CDataSection(const lexer::SourcePos& sourcePos_, const std::string& text_);
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
};

CDataSection* MakeCDataSection(const std::string& text);

} // namespace xml
