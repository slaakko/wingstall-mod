// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module rex.match;

import std.core;

export namespace rex {

class Nfa;
class Context;

bool FilePatternMatch(const std::u32string& filePath, const std::u32string& filePattern);
bool PatternMatch(const std::u32string& str, const std::u32string& regularExpressionPattern);
bool PatternMatch(const std::u32string& str, Nfa& nfa);
Nfa CompileRegularExpressionPattern(Context& context, const std::u32string& regularExpressionPattern);
Nfa CompileFilePattern(Context& context, const std::u32string& filePattern);

} // namespace soulng::rex
