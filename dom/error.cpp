// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module dom.error;

namespace xml {

XmlException::XmlException(const std::string& errorMessage_, const lexer::SourcePos& sourcePos_) : std::runtime_error(errorMessage_), sourcePos(sourcePos_)
{
}

} // namespace xml
