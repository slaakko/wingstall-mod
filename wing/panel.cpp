// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing.panel;

namespace wing {

ControlCreateParams& MakePanelCreateParams(ControlCreateParams& createParams)
{
    if (createParams.windowClassName.empty())
    {
        createParams.WindowClassName("wing.Panel");
    }
    createParams.WindowStyle(DefaultChildWindowStyle());
    return createParams;
}

Panel::Panel(ControlCreateParams& createParams) : ContainerControl(MakePanelCreateParams(createParams))
{
}

} // wing
