// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing_gui.message_dialog;

namespace wing_gui {

std::string GetMessageWindowCaptionText(MessageKind messageKind)
{
    switch (messageKind)
    {
    case MessageKind::info: return "Information";
    case MessageKind::error: return "Error";
    }
    return std::string();
}

MessageDialog::MessageDialog(MessageKind messageKind, const std::string& messageText) : wing::Window(wing::WindowCreateParams().Text(
    GetMessageWindowCaptionText(messageKind)).WindowStyle(wing::DialogWindowStyle()).
    WindowClassName("wing_gui.MessageDialog").WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).BackgroundColor(wing::DefaultControlBackgroundColor()).
    Location(wing::DefaultLocation()).SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(160), wing::ScreenMetrics::Get().MMToVerticalPixels(60)))),
    okButton(nullptr)
{
    wing::Size s = GetSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();

    wing::Point labelLocation(16, 16);
    std::unique_ptr<wing::Label> labelPtr(new wing::Label(wing::LabelCreateParams().Text(messageText).Location(labelLocation).SetSize(
        wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(100), wing::ScreenMetrics::Get().MMToVerticalPixels(40))).AutoSize(false).
        SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    AddChild(labelPtr.release());

    int x = s.Width - defaultButtonSize.Width - defaultControlSpacing.Width;
    int y = s.Height - defaultButtonSize.Height - defaultControlSpacing.Height;

    std::unique_ptr<wing::Button> okButtonPtr(new wing::Button(wing::ControlCreateParams().Location(wing::Point(x, y)).SetSize(defaultButtonSize).Text("OK").
        SetAnchors(wing::Anchors::right | wing::Anchors::bottom)));
    okButton = okButtonPtr.get();
    okButton->SetDialogResult(wing::DialogResult::ok);
    okButton->SetDefault();
    SetDefaultButton(okButton);
    AddChild(okButtonPtr.release());
    okButton->SetFocus();
}

} // namespace wing_gui
