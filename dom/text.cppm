// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module dom.text;

import std.core;
import dom.character_data;
import dom.node;

export namespace xml {

class Text : public CharacterData
{
public:
    Text(const lexer::SourcePos& sourcePos_);
    Text(const lexer::SourcePos& sourcePos_, const std::string& text_);
    Text(NodeKind kind_, const lexer::SourcePos& sourcePos_, const std::string& name_);
    Text(NodeKind kind_, const lexer::SourcePos& sourcePos_, const std::string& name_, const std::string& text_);
    void Accept(Visitor& visitor) override;
};

Text* MakeText(const std::string& text);

} // namespace xml
