// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module rex.algorithm;

import std.core;

export namespace rex {

class NfaState;
class Nfa;

std::vector<NfaState*> EpsilonClosure(const std::vector<NfaState*>& states);
std::vector<NfaState*> EpsilonClosure(NfaState* state);
std::vector<NfaState*> Move(const std::vector<NfaState*>& states, char32_t c);
bool Match(Nfa& nfa, const std::u32string& s);

} // namespace rex
