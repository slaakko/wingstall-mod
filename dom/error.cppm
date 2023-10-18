// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module dom.error;

import std.core;
import lexer.source_pos;

export namespace xml {

class XmlException : public std::runtime_error
{
public:
    XmlException(const std::string& errorMessage_, const lexer::SourcePos& sourcePos_);
    const lexer::SourcePos& GetSourcePos() const { return sourcePos; }
private:
    lexer::SourcePos sourcePos;
};

} // namespace soul::xml
