// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.console;

import wing.ansi;
import wing.color;
import wing.control;
import wing.base;
import wing.event;
import wing.text_view;
import wing.graphics;
import std.core;

export namespace wing {

using ConsoleInputReadyEvent = Event;

ConsoleColor DefaultConsoleBackColor();
ConsoleColor DefaultConsoleTextColor();

struct ColorCount
{
    ColorCount();
    ColorCount(ConsoleColor color_, int count_);
    void IncrementCount() { ++count; }
    void DecrementCount() { --count; }
    ConsoleColor color;
    int count;
};

struct ConsoleCreateParams
{
    ConsoleCreateParams();
    ConsoleCreateParams& Defaults();
    ConsoleCreateParams& WindowClassName(const std::string& windowClassName_);
    ConsoleCreateParams& WindowClassStyle(uint32_t windowClassStyle_);
    ConsoleCreateParams& WindowStyle(int windowStyle_);
    ConsoleCreateParams& WindowClassBackgroundColor(int windowClassBackgroundColor_);
    ConsoleCreateParams& BackgroundColor(const Color& backgroundColor_);
    ConsoleCreateParams& Text(const std::string& text_);
    ConsoleCreateParams& Location(const Point& location_);
    ConsoleCreateParams& SetSize(const Size& size_);
    ConsoleCreateParams& SetAnchors(Anchors anchors_);
    ConsoleCreateParams& SetDock(Dock dock_);
    ConsoleCreateParams& FontFamilyName(const std::string& fontFamilyName_);
    ConsoleCreateParams& FontSize(float fontSize_);
    ConsoleCreateParams& SetPadding(const Padding& padding_);
    ConsoleCreateParams& DefaultBackColor(ConsoleColor color);
    ConsoleCreateParams& DefaultTextColor(ConsoleColor color);
    TextViewCreateParams textViewCreateParams;
    Padding padding;
    ConsoleColor defaultBackColor;
    ConsoleColor defaultTextColor;
};

class Console : public TextView
{
public:
    Console(ConsoleCreateParams& createParams);
    void Write(int handle, const std::string& text);
    void StartReadLine();
    void Clear() override;
    ConsoleInputReadyEvent& ConsoleInputReady() { return consoleInputReady; }
    bool Eof() const { return eof; }
    const std::u32string& InputLine() const { return inputLine; }
protected:
    void PaintContent(Graphics& graphics, const Rect& clipRect) override;
    void OnKeyDown(KeyEventArgs& args) override;
    void OnKeyPress(KeyPressEventArgs& args) override;
    virtual void OnConsoleInputReady();
private:
    void OutputChar(ConsoleColor textColor, ConsoleColor backColor, int handle, char32_t c);
    void AddColor(ConsoleColor color, std::vector<ColorCount>& colorLine);
    void PaintLineBackground(Graphics& graphics, const std::vector<ColorCount>& backColorLine, const PointF& origin);
    void DrawLineText(Graphics& graphics, const std::u32string& line, const std::vector<ColorCount>& textColorLine, const PointF& origin);
    void InsertChar(char32_t ch);
    void DeleteChar();
    void IncrementCaretColorCount();
    void IncrementColorCount(std::vector<ColorCount>& colorLine, int caretCol);
    void DecrementCaretColorCount();
    void DecrementColorCount(std::vector<ColorCount>& colorLine, int caretCol);
    SolidBrush* GetOrInsertBrush(ConsoleColor color);
    AnsiEngine outEngine;
    AnsiEngine errorEngine;
    std::map<ConsoleColor, SolidBrush*> brushMap;
    std::vector<std::unique_ptr<SolidBrush>> brushes;
    std::vector<std::vector<ColorCount>> textColorLines;
    std::vector<std::vector<ColorCount>> backColorLines;
    Padding padding;
    ConsoleColor defaultBackColor;
    ConsoleColor defaultTextColor;
    std::u32string inputLine;
    bool eof;
    int startInputCol;
    ConsoleInputReadyEvent consoleInputReady;
};

} // wing
