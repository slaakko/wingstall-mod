// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module dom.character_data;

import std.core;
import dom.node;

export namespace xml {

std::string XmlCharDataEscape(const std::string& text);

class CharacterData : public Node
{
public:
    CharacterData(NodeKind kind_, const lexer::SourcePos& sourcePos_, const std::string& name_);
    CharacterData(NodeKind kind_, const lexer::SourcePos& sourcePos_, const std::string& name_, const std::string& data_);
    const std::string& Data() const { return data; }
    std::string& Data() { return data; }
    void Write(util::CodeFormatter& formatter) override;
    bool ValueContainsNewLine() const final;
private:
    std::string data;
};

} // namespace xml
