// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module xpath.xpath_token_value_parsers;

import util;

namespace xpath {

std::string ParseDQString(const std::string& fileName, const lexer::Token& token)
{
    std::u32string s;
    const char32_t* p = token.match.begin;
    const char32_t* e = token.match.end;
    if (p != e && *p == '"')
    {
        ++p;
    }
    while (p != e && *p != '"')
    {
        s.append(1, *p);
        ++p;
    }
    if (p != e && *p == '"')
    {
        ++p;
    }
    if (p != e)
    {
        throw std::runtime_error("invalid string literal at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(token.match.ToString()));
    }
    return util::ToUtf8(s);
}

std::string ParseSQString(const std::string& fileName, const lexer::Token& token)
{
    std::u32string s;
    const char32_t* p = token.match.begin;
    const char32_t* e = token.match.end;
    if (p != e && *p == '\'')
    {
        ++p;
    }
    while (p != e && *p != '\'')
    {
        s.append(1, *p);
        ++p;
    }
    if (p != e && *p == '\'')
    {
        ++p;
    }
    if (p != e)
    {
        throw std::runtime_error("invalid string literal at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(token.match.ToString()));
    }
    return util::ToUtf8(s);
}

} // namespace xpath
