// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module rex.match;

import rex.context;
import rex.lexer;
import rex.parser;
import rex.algorithm;
import rex.nfa;

namespace rex {

std::u32string MakeRegularExpressionPatternFromFilePattern(const std::u32string& filePattern)
{
    std::u32string pattern;
    for (char32_t c : filePattern)
    {
        switch (c)
        {
        case '.': pattern.append(U"\\."); break;
        case '*': pattern.append(U".*"); break;
        case '?': pattern.append(U"."); break;
        default: pattern.append(1, c); break;
        }
    }
    return pattern;
}

bool FilePatternMatch(const std::u32string& filePath, const std::u32string& filePattern)
{
    return PatternMatch(filePath, MakeRegularExpressionPatternFromFilePattern(filePattern));
}

bool PatternMatch(const std::u32string& str, const std::u32string& regularExpressionPattern)
{
    Context context;
    Nfa nfa = CompileRegularExpressionPattern(context, regularExpressionPattern);
    return PatternMatch(str, nfa);
}

Nfa CompileRegularExpressionPattern(Context& context, const std::u32string& regularExpressionPattern)
{
    RexLexer lexer(regularExpressionPattern, "", 0);
    return RexParser::Parse(lexer, &context);
}

Nfa CompileFilePattern(Context& context, const std::u32string& filePattern)
{
    return CompileRegularExpressionPattern(context, MakeRegularExpressionPatternFromFilePattern(filePattern));
}

bool PatternMatch(const std::u32string& str, Nfa& nfa)
{
    return Match(nfa, str);
}

} // namespace rex
