// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module dom.entity_reference;

import dom.visitor;

namespace xml {

EntityReference::EntityReference(const lexer::SourcePos& sourcePos_) :
    CharacterData(NodeKind::entityReferenceNode, sourcePos_, "entity_reference")
{
}

EntityReference::EntityReference(const lexer::SourcePos& sourcePos_, const std::string& entityRef_) :
    CharacterData(NodeKind::entityReferenceNode, sourcePos_, "entity_reference", entityRef_)
{
}

void EntityReference::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void EntityReference::Write(util::CodeFormatter& formatter)
{
    formatter.Write("&");
    formatter.Write(Data());
    formatter.Write(";");
}

EntityReference* MakeEntityReference(const std::string& entityRef)
{
    return new EntityReference(lexer::SourcePos(), entityRef);
}

} // namespace xml
