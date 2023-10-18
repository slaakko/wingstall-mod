// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.tool_bar;

import wing;
import std.core;

export namespace package_editor {

wing::Color DefaultToolBarBorderColor();
wing::Color DefaultToolBarTextButtonColor();
wing::Color DefaultToolBarToolTipColor();
int DefaultToolBarToolButtonHeight();
wing::Padding DefaultToolBarPadding();
wing::Padding DefaultToolBarToolButtonPadding();

wing::ToolBar* MakeToolBar();

} // namespace package_editor
