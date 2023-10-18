// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module xpath.xpath_token_value_parsers;

import lexer;
import std.core;

export namespace xpath {

std::string ParseDQString(const std::string& fileName, const lexer::Token& token);
std::string ParseSQString(const std::string& fileName, const lexer::Token& token);

}
