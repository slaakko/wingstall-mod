// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing_gui.message_dialog;

import wing;

export namespace wing_gui {

enum class MessageKind
{
    info, error
};

class MessageDialog : public wing::Window
{
public:
    MessageDialog(MessageKind messageKind, const std::string& messageText);
private:
    wing::Button* okButton;
};

} // namespace wing_gui
