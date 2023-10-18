// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module rex.symbol;

import std.core;

export namespace rex {

const char32_t eps = '\0';

class Symbol
{
public:
    virtual ~Symbol();
    virtual bool Match(char32_t c) = 0;
};

class Char : public Symbol
{
public:
    Char(char32_t chr_);
    bool Match(char32_t c) override;
    char32_t Chr() const { return chr; }
private:
    char32_t chr;
};

class Any : public Symbol
{
public:
    bool Match(char32_t c) override;
};

class Range : public Symbol
{
public:
    Range(char32_t start_, char32_t end_);
    bool Match(char32_t c) override;
    char32_t Start() const { return start; }
    char32_t End() const { return end; }
private:
    char32_t start;
    char32_t end;
};

inline bool operator==(const Range& left, const Range& right)
{
    return left.Start() == right.Start() && left.End() == right.End();
}

inline bool operator<(const Range& left, const Range& right)
{
    if (left.Start() < right.Start()) return true;
    if (left.Start() > right.Start()) return false;
    return left.End() < right.End();
}

class Class : public Symbol
{
public:
    Class();
    bool Match(char32_t c) override;
    void SetInverse() { inverse = true; }
    void AddSymbol(Symbol* symbol);
private:
    bool inverse;
    std::vector<Symbol*> symbols;
};

} // namespace rex
