// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

module package_editor.tool_bar;

namespace package_editor {

wing::Color DefaultToolBarBorderColor()
{
    return wing::Color(204, 206, 219);
}

wing::Color DefaultToolBarTextButtonColor()
{
    return wing::Color(0, 78, 170);
}

wing::Color DefaultToolBarToolTipColor()
{
    return wing::Color(241, 242, 247);
}

int DefaultToolBarToolButtonHeight()
{
    return 30;
}

wing::Padding DefaultToolBarPadding()
{
    return wing::Padding(4, 4, 4, 4);
}

wing::Padding DefaultToolBarToolButtonPadding()
{
    return wing::Padding(2, 0, 2, 0);
}

wing::ToolBar* MakeToolBar()
{
    return new wing::ToolBar(wing::ToolBarCreateParams().SetDock(wing::Dock::none).ToolButtonHeight(DefaultToolBarToolButtonHeight()).
        ToolBarPadding(DefaultToolBarPadding()).ToolBarToolButtonPadding(DefaultToolBarToolButtonPadding()).ToolTipWindowColor(DefaultToolBarToolTipColor()));
}

} // namespace package_editor
