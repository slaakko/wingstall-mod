// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.file;

import package_editor.node;
import dom;
import wing;
import std.core;

export namespace package_editor {

class File : public Node
{
public:
    File(const std::string& name_);
    File(const std::string& packageXMLFilePath, xml::Element* element);
    xml::Element* ToXml() const;
    std::string ImageName() const override { return "file.bitmap"; }
    bool Leaf() const override { return true; }
    bool CanOpen() const override { return false; }
    wing::TreeViewNode* ToTreeViewNode(wing::TreeView* view);
};

} // package_editor
