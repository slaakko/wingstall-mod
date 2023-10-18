// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module dom.entity_reference;

import std.core;
import dom.character_data;

export namespace xml {

class EntityReference : public CharacterData
{
public:
    EntityReference(const lexer::SourcePos& sourcePos_);
    EntityReference(const lexer::SourcePos& sourcePos_, const std::string& entityRef_);
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
};

EntityReference* MakeEntityReference(const std::string& entityRef);

} // namespace xml
