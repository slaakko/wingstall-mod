// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module lexer.parsing_exception;

namespace lexer {

ParsingException::ParsingException(const std::string& message_, const std::string& fileName_, const Span& span_) :
    std::runtime_error(message_), message(message_), fileName(fileName_), span(span_), mod(nullptr)
{
}

ParsingException::ParsingException(const std::string& message_, const std::string& fileName_) : 
    std::runtime_error(message_), message(message_), fileName(fileName_), span(), mod(nullptr)
{
}

void ParsingException::SetProject(const std::string& project_)
{
    project = project_;
}

} // namespace lexer
