// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.uninstall_component;

import dom;
import std.core;

export namespace package_editor {

class UninstallComponent
{
public:
    UninstallComponent(const std::string& packageXMLFilePath, xml::Element* element);
    xml::Element* ToXml() const;
private:
    std::vector<std::string> runCommands;
};

} // package_editor
