// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing_gui.action_dialog;

namespace wing_gui {

std::string GetActionDialogKindStr(ActionDialogKind kind)
{
    switch (kind)
    {
    case ActionDialogKind::installationAction: return "Installation";
    case ActionDialogKind::uninstallationAction: return "Uninstallation";
    }
    return std::string();
}

std::string GetActionStr(ActionDialogKind kind)
{
    switch (kind)
    {
    case ActionDialogKind::installationAction: return "installation";
    case ActionDialogKind::uninstallationAction: return "uninstallation";
    }
    return std::string();
}

ActionDialog::ActionDialog(ActionDialogKind kind) : wing::Window(wing::WindowCreateParams().Text(
    GetActionDialogKindStr(kind) + " actions").WindowStyle(wing::DialogWindowStyle()).WindowClassName("wing_gui.ActionDialog").
    WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).BackgroundColor(wing::DefaultControlBackgroundColor()).
    Location(wing::DefaultLocation()).SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(160), wing::ScreenMetrics::Get().MMToVerticalPixels(60)))),
    abortButton(nullptr), continueButton(nullptr)
{
    wing::Size s = GetSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();

    wing::Point abortLabelLocation(16, 16);
    std::unique_ptr<wing::Label> abortLabelPtr(new wing::Label(wing::LabelCreateParams().Location(abortLabelLocation).
        Text("Abort action: no work is undone, the system is left as it is in the middle of " + GetActionStr(kind) + ".").SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    AddChild(abortLabelPtr.release());

    wing::Point continueLabelLocation(16, 16 + 24);
    std::unique_ptr<wing::Label> continueLabelPtr(new wing::Label(wing::LabelCreateParams().Location(continueLabelLocation).
        Text("Continue action: continue " + GetActionStr(kind) + ".").SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    AddChild(continueLabelPtr.release());

    int x = s.Width - defaultButtonSize.Width - defaultControlSpacing.Width;
    int y = s.Height - defaultButtonSize.Height - defaultControlSpacing.Height;

    std::unique_ptr<wing::Button> continueButtonPtr(new wing::Button(wing::ControlCreateParams().Location(wing::Point(x, y)).SetSize(defaultButtonSize).Text("Continue").
        SetAnchors(wing::Anchors::right | wing::Anchors::bottom)));
    continueButton = continueButtonPtr.get();
    continueButton->SetDialogResult(wing::DialogResult::ok);
    continueButton->SetDefault();
    SetDefaultButton(continueButton);
    AddChild(continueButtonPtr.release());
    x = x - defaultButtonSize.Width - defaultControlSpacing.Width;

    std::unique_ptr<wing::Button> abortButtonPtr(new wing::Button(wing::ControlCreateParams().Location(wing::Point(x, y)).SetSize(defaultButtonSize).Text("Abort").
        SetAnchors(wing::Anchors::right | wing::Anchors::bottom)));
    abortButton = abortButtonPtr.get();
    abortButton->SetDialogResult(wing::DialogResult::abort);
    AddChild(abortButtonPtr.release());

    continueButton->SetFocus();
}

} // namespace wing_gui
