// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module dom.text;

import dom.visitor;

namespace xml {

Text::Text(const lexer::SourcePos& sourcePos_) : CharacterData(NodeKind::textNode, sourcePos_, "text")
{
}

Text::Text(const lexer::SourcePos& sourcePos_, const std::string& text_) : CharacterData(NodeKind::textNode, sourcePos_, "text", text_)
{
}

Text::Text(NodeKind kind_, const lexer::SourcePos& sourcePos_, const std::string& name_) : CharacterData(kind_, sourcePos_, name_)
{
}

Text::Text(NodeKind kind_, const lexer::SourcePos& sourcePos_, const std::string& name_, const std::string& text_) : CharacterData(kind_, sourcePos_, name_, text_)
{
}

void Text::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Text* MakeText(const std::string& text)
{
    return new Text(lexer::SourcePos(), text);
}

} // namespace xml
