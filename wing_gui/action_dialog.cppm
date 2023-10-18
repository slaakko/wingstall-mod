// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing_gui.action_dialog;

import wing;
import std.core;

export namespace wing_gui {

enum class ActionDialogKind
{
    installationAction, uninstallationAction
};

class ActionDialog : public wing::Window
{
public:
    ActionDialog(ActionDialogKind kind);
private:
    wing::Button* abortButton;
    wing::Button* continueButton;
};

} // namespace wing_gui
