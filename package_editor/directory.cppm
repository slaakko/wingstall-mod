// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.directory;

import package_editor.content;
import package_editor.node;
import package_editor.rule;
import wing;
import dom;
import std.core;

export namespace package_editor {

class Directory : public Node
{
public:
    Directory(const std::string& name_);
    Directory(const std::string& packageXMLFilePath, xml::Element* element);
    xml::Element* ToXml() const;
    std::string ImageName() const override { return "folder.closed.bitmap"; }
    std::string ViewName() const override { return "directory_view"; }
    wing::TreeViewNode* ToTreeViewNode(wing::TreeView* view);
    wing::Control* CreateView(wing::ImageList* imageList) override;
    std::string DirectoryPath() const override;
    int RuleCount() const override;
    Rule* GetRule(int index) const override;
    Rule* GetRule(const std::string& name) const override;
    Node* GetRuleContainerNode() override;
private:
    std::unique_ptr<Rules> rules;
    std::unique_ptr<Content> content;
};

} // package_editor
