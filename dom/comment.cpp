// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module dom.comment;

import dom.visitor;

namespace xml {

Comment::Comment(const lexer::SourcePos& sourcePos_) : CharacterData(NodeKind::commentNode, sourcePos_, "comment")
{
}

Comment::Comment(const lexer::SourcePos& sourcePos_, const std::string& comment_) : CharacterData(NodeKind::commentNode, sourcePos_, "comment", comment_)
{
}

void Comment::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void Comment::Write(util::CodeFormatter& formatter)
{
    formatter.Write("<!-- ");
    formatter.Write(Data());
    formatter.Write(" -->");
}

Comment* MakeComment(const std::string& comment)
{
    return new Comment(lexer::SourcePos(), comment);
}

} // namespace xml
