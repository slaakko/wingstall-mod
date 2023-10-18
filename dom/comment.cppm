// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module dom.comment;

import std.core;
import dom.character_data;

export namespace xml {

class Comment : public CharacterData
{
public:
    Comment(const lexer::SourcePos& sourcePos_);
    Comment(const lexer::SourcePos& sourcePos_, const std::string& comment_);
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
};

Comment* MakeComment(const std::string& comment);

} // namespace xml
