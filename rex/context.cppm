// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module rex.context;

import rex.symbol;
import std.core;

export namespace rex {

class NfaState;

class Context
{
public:
    Context();
    ~Context();
    NfaState* MakeNfaState();
    Symbol* MakeChar(char32_t c);
    Symbol* MakeRange(char32_t start, char32_t end);
    Symbol* MakeAny() { return &any; }
    Symbol* MakeEpsilon() { return &epsilon; }
    Class* MakeClass();
private:
    std::vector<NfaState*> nfaStates;
    std::vector<Symbol*> symbols;
    Any any;
    Char epsilon;
    std::map<char32_t, Symbol*> charSymbols;
    std::map<Range, Symbol*> rangeSymbols;
};

} // namespace rex
