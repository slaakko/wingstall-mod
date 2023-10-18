// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module package_editor.about_dialog;

namespace package_editor {

std::string WingstallVersionStr()
{
    return "3.0.0";
}

AboutDialog::AboutDialog() :
    wing::Window(wing::WindowCreateParams().WindowClassName("package_editor.about_dialog").Location(wing::DefaultLocation()).WindowStyle(wing::DialogWindowStyle()).Text("About").
        WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).BackgroundColor(wing::DefaultControlBackgroundColor()).
        SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(100), wing::ScreenMetrics::Get().MMToVerticalPixels(80))))
{
    SetCaretDisabled();
    wing::Size s = GetSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();
    wing::Size defaultLabelSize = wing::ScreenMetrics::Get().DefaultLabelSize();

    std::u16string fontFamilyName = u"Segoe UI"; // todo
    wing::Font font(wing::FontFamily((const WCHAR*)fontFamilyName.c_str()), 14.0f, wing::FontStyle::FontStyleRegular, wing::Unit::UnitPoint);
    std::unique_ptr<wing::Label> label(new wing::Label(wing::LabelCreateParams().Text("Wingstall version " + std::string(WingstallVersionStr()) + "\nCopyright (c) 2023 Seppo Laakko").
        Location(wing::Point(16, 16)).SetSize(defaultLabelSize).SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    label->SetFont(std::move(font));
    AddChild(label.release());

    int x = s.Width - defaultButtonSize.Width - defaultControlSpacing.Width;
    int y = s.Height - defaultButtonSize.Height - defaultControlSpacing.Height;

    std::unique_ptr<wing::Button> okButtonPtr(new wing::Button(wing::ControlCreateParams().Location(wing::Point(x, y)).SetSize(defaultButtonSize).Text("OK").
        SetAnchors(wing::Anchors::right | wing::Anchors::bottom)));
    okButtonPtr->SetDialogResult(wing::DialogResult::ok);
    okButtonPtr->SetDefault();
    SetDefaultButton(okButtonPtr.get());
    AddChild(okButtonPtr.release());
}

} // package_editor
