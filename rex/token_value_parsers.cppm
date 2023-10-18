// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module rex.token_value_parsers;

import lexer;
import std.core;

export namespace rex {

char32_t MakeEscapeValue(const std::string& fileName, const lexer::Token& token);

} // namespace rex
