// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>
#include <Gdiplus.h>

export module wing.graphics;

import wing.base;
import std.core;

export namespace wing {

using Color = Gdiplus::Color;
using Point = Gdiplus::Point;
using PointF = Gdiplus::PointF;
using Size = Gdiplus::Size;
using SizeF = Gdiplus::SizeF;
using Rect = Gdiplus::Rect;
using RectF = Gdiplus::RectF;
using FontFamily = Gdiplus::FontFamily;
using FontStyle = Gdiplus::FontStyle;
using Unit = Gdiplus::Unit;
using Graphics = Gdiplus::Graphics;
using GraphicsState = Gdiplus::GraphicsState;
using StringFormat = Gdiplus::StringFormat;
using StringAlignment = Gdiplus::StringAlignment;
using HotKeyPrefix = Gdiplus::HotkeyPrefix;
using Bitmap = Gdiplus::Bitmap;
using Matrix = Gdiplus::Matrix;
using Region = Gdiplus::Region;
using Brush = Gdiplus::Brush;
using SolidBrush = Gdiplus::SolidBrush;
using Pen = Gdiplus::Pen;

class GdiPlusException : public std::runtime_error
{
public:
    GdiPlusException(Gdiplus::Status status_);
    Gdiplus::Status Status() const { return status; }
private:
    Gdiplus::Status status;
};

void CheckGraphicsStatus(Gdiplus::Status status);

class Font
{
public:
    Font() : nativeFont(nullptr) {}
    ~Font();
    Font(const FontFamily& family, float size, FontStyle style, Unit unit);
    Font(const Font& that);
    Font(Font&& that) noexcept;
    Font& operator=(const Font& that);
    Font& operator=(Font&& that) noexcept;
    bool IsNull() const { return nativeFont == nullptr; }
    Gdiplus::Font* NativeFont() const { return nativeFont; }
private:
    Gdiplus::Font* nativeFont;
};

class FontHandle
{
public:
    FontHandle(HFONT handle_);
    ~FontHandle();
    FontHandle(const FontHandle&) = delete;
    FontHandle(FontHandle&& that) noexcept;
    FontHandle& operator=(const FontHandle&) = delete;
    FontHandle& operator=(FontHandle&& that) noexcept;
    bool IsNull() const { return handle == nullptr; }
    HFONT Handle() const { return handle; }
private:
    HFONT handle;
};

FontHandle ToFontHandle(Graphics& graphics, const Font& font);

inline bool operator==(const Point& p, const Point& q)
{
    return p.X == q.X && p.Y == q.Y;
}

inline bool operator!=(const Point& p, const Point& q)
{
    return !(p == q);
}

inline bool operator==(const Size& s, const Size& t)
{
    return s.Width == t.Width && s.Height == t.Height;
}

inline bool operator!=(const Size& s, const Size& t)
{
    return !(s == t);
}

inline bool operator==(const Color& left, const Color& right)
{
    return left.GetValue() == right.GetValue();
}

inline bool operator!=(const Color& left, const Color& right)
{
    return !(left == right);
}

Point GetMessagePos();
Rect ToRect(const RECT& winRect);
RECT ToWinRect(const Rect& rect);
Size LParamSize(Message& msg);
Point LParamLocation(Message& msg);
HWND LParamHandle(Message& msg);

void DrawString(Graphics& graphics, const std::string& text, const Font& font, const PointF& origin, const Brush& brush);
void DrawString(Graphics& graphics, const std::string& text, const Font& font, const PointF& origin, const StringFormat& stringFormat, const Brush& brush);
void DrawString(Graphics& graphics, const std::string& text, const Font& font, const RectF& rect, const StringFormat& stringFormat, const Brush& brush);
RectF MeasureString(Graphics& graphics, const std::string& text, const Font& font, const PointF& origin, const StringFormat& stringFormat);

Color ToGray(const Color& color, const Color& transparentColor);
std::unique_ptr<Bitmap> ToGrayBitmap(Bitmap* bm, const Color& transparentColor);

} // wing
