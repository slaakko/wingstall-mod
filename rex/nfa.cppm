// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module rex.nfa;

import rex.symbol;
import std.core;

export namespace rex {

class NfaState;
class Context;

class NfaEdge
{
public:
    NfaEdge(Symbol* symbol_, NfaState* next_);
    Symbol* GetSymbol() const { return symbol; }
    NfaState* Next() const { return next; }
private:
    Symbol* symbol;
    NfaState* next;
};

class NfaState
{
public:
    NfaState();
    bool Accept() const { return accept; }
    void SetAccept(bool accept_) { accept = accept_; }
    const std::vector<NfaEdge>& Edges() const { return edges; }
    void SetEdges(const std::vector<NfaEdge>& edges_);
    void AddEdge(const NfaEdge& edge);
    std::vector<NfaState*> Next(char32_t c) const;
private:
    bool accept;
    std::vector<NfaEdge> edges;
};

class Nfa
{
public:
    Nfa();
    Nfa(NfaState* start_, NfaState* end_);
    NfaState* Start() const { return start; }
    void SetStart(NfaState* start_) { start = start_; }
    NfaState* End() const { return end; }
    void SetEnd(NfaState* end_) { end = end_; }
private:
    NfaState* start;
    NfaState* end;
};

Nfa MakeNfa(Context& context, Symbol* symbol);
Nfa Cat(const Nfa& left, const Nfa& right);
Nfa Alt(Context& context, const Nfa& left, const Nfa& right);
Nfa Kleene(Context& context, const Nfa& nfa);
Nfa Pos(Context& context, const Nfa& nfa);
Nfa Opt(Context& context, const Nfa& nfa);

} // namespace rex
