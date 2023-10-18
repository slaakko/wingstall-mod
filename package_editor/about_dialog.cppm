// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.about_dialog;

import wing;
import std.core;

export namespace package_editor {

std::string WingstallVersionStr();

class AboutDialog : public wing::Window
{
public:
    AboutDialog();
};

} // package_editor
