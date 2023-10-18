// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module lexer.parsing_exception;

import lexer.span;
import std.core;

export namespace lexer {

class ParsingException : public std::runtime_error
{
public:
    ParsingException(const std::string& message_, const std::string& fileName_, const Span& span_);
    ParsingException(const std::string& message_, const std::string& fileName_);
    const std::string& Project() const { return project; }
    const std::string& Message() const { return message; }
    const std::string& FileName() const { return fileName; }
    const Span& GetSpan() const { return span; }
    void SetProject(const std::string& project_);
    void* Module() const { return mod; }
    void SetModule(void* mod_) { mod = mod_; }
private:
    std::string project;
    std::string message;
    std::string fileName;
    Span span;
    void* mod;
};

} // namespace lexer
