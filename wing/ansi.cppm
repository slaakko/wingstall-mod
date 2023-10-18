// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.ansi;

import wing.color;
import util;
import std.core;

export namespace wing {

using ColorOutputFn = std::function<void(ConsoleColor, ConsoleColor, int, char32_t)>;

class ColorCharOutputMethod
{
public:
    template<class T>
    void Set(T* t, void (T::* pm)(ConsoleColor textColor, ConsoleColor backColor, int handle, char32_t c))
    {
        fn = std::bind(pm, t, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    }
    void operator()(ConsoleColor textColor, ConsoleColor backColor, int handle, char32_t c)
    {
        fn(textColor, backColor, handle, c);
    }
private:
    ColorOutputFn fn;
};

class AnsiEngine
{
public:
    AnsiEngine(int handle_);
    void SetColorCharOutputMethod(const ColorCharOutputMethod& outputMethod_);
    void Process(const std::string& text);
private:
    void Put(char c);
    void Emit(char c);
    void ResetColors();
    void SetColors();
    int handle;
    int state;
    ConsoleColor textColor;
    ConsoleColor backColor;
    ConsoleColor currentTextColor;
    ConsoleColor currentBackColor;
    ConsoleColor defaultTextColor;
    ConsoleColor defaultBackColor;
    util::Utf8ToUtf32Engine unicodeEngine;
    ColorCharOutputMethod outputMethod;
};

} // wing
