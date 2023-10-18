// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.metrics;

import wing.graphics;
import dom.element;
import std.core;

export namespace wing {

const float inchMM = 25.4f;
const float pointMM = 0.351450f;

inline float InchToMM(float inches)
{
    return inches * inchMM;
}

inline float MMToInch(float mm)
{
    return mm / inchMM;
}

inline float PointToMM(float points)
{
    return points * pointMM;
}

inline float MMToPoint(float mm)
{
    return mm / pointMM;
}

inline int MMToPixels(float mm, float dpi)
{
    return static_cast<int>(mm * dpi / inchMM);
}

inline float PixelsToMM(int pixels, float dpi)
{
    return pixels * inchMM / dpi;
}

std::string GetDefaultMetricsFilePath();

class SizeElement
{
public:
    SizeElement(const std::string& name_);
    const std::string& Name() const { return name; }
    const SizeF& Get() const { return size; }
    void Set(const SizeF& size_) { size = size_; }
    void Read(xml::Element* parentElement);
    void Write(xml::Element* parentElement);
private:
    std::string name;
    SizeF size;
};

class Metrics
{
public:
    Metrics();
    void SetElementValues();
    void LoadFromFile(const std::string& fileName);
    void SaveToFile(const std::string& fileName, bool setElementValues);
    void Calculate(Graphics& graphics);
    float DPIX() const { return dpiX; }
    float DPIY() const { return dpiY; }
    int MMToHorizontalPixels(float mm) const { return MMToPixels(mm, dpiX); }
    int HorizontalPixelsToMM(int pixels) const { return PixelsToMM(pixels, dpiX); }
    int MMToVerticalPixels(float mm) const { return MMToPixels(mm, dpiY); }
    int VerticalPixelsToMM(int pixels) const { return PixelsToMM(pixels, dpiY); }
    const Size& DefaultButtonSize() const { return defaultButtonSize; }
    void SetDefaultButtonSize(const Size& defaultButtonSize_) { defaultButtonSize = defaultButtonSize_; }
    const Size& DefaultLabelSize() const { return defaultLabelSize; }
    void SetDefaultLabelSize(const Size& defaultLabelSize_) { defaultLabelSize = defaultLabelSize_; }
    const Size& DefaultTextBoxSize() const { return defaultTextBoxSize; }
    void SetDefaultTextBoxSize(const Size& defaultTextBoxSize_) { defaultTextBoxSize = defaultTextBoxSize_; }
    const Size& DefaultComboBoxSize() const { return defaultComboBoxSize; }
    void SetDefaultComboBoxSize(const Size& defaultComboBoxSize_) { defaultComboBoxSize = defaultComboBoxSize_; }
    const Size& DefaultListBoxSize() const { return defaultListBoxSize; }
    void SetDefaultListBoxSize(const Size& defaultListBoxSize_) { defaultListBoxSize = defaultListBoxSize_; }
    const Size& DefaultCheckBoxSize() const { return defaultCheckBoxSize; }
    void SetDefaultCheckBoxSize(const Size& defaultCheckBoxSize_) { defaultCheckBoxSize = defaultCheckBoxSize_; }
    const Size& DefaultControlSpacing() const { return defaultControlSpacing; }
    void SetDefaultControlSpacing(const Size& defaultControlSpacing_) { defaultControlSpacing = defaultControlSpacing_; }
private:
    void Read(xml::Element* parentElement);
    void Write(xml::Element* parentElement);
    float dpiX;
    float dpiY;
    Size defaultButtonSize;
    SizeElement defaultButtonSizeElement;
    Size defaultLabelSize;
    SizeElement defaultLabelSizeElement;
    Size defaultTextBoxSize;
    SizeElement defaultTextBoxSizeElement;
    Size defaultComboBoxSize;
    SizeElement defaultComboBoxSizeElement;
    Size defaultListBoxSize;
    SizeElement defaultListBoxSizeElement;
    Size defaultCheckBoxSize;
    SizeElement defaultCheckBoxSizeElement;
    Size defaultControlSpacing;
    SizeElement defaultControlSpacingElement;
};

class ScreenMetrics
{
public:
    static void Load();
    static void Save(bool setElementValues);
    static void Calculate(Graphics& graphics);
    static Metrics& Get() { return metrics; }
    static void Set(Metrics& metrics_);
    static bool Calculated() { return calculated; }
    static void SetCalculated() { calculated = true; }
private:
    static Metrics metrics;
    static bool calculated;
};

void LoadMetrics();

} // wing
