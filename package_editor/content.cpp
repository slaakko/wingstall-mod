// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module package_editor.content;

import package_editor.main_window;
import package_editor.add_directories_and_files_dialog;
import package_editor.package;
import util;

namespace package_editor {

Content::Content() : Node(NodeKind::content, "Content")
{
}

wing::TreeViewNode* Content::ToTreeViewNode(wing::TreeView* view)
{
    wing::TreeViewNode* node = new wing::TreeViewNode("Content");
    SetTreeViewNode(node);
    node->SetData(this);
    wing::ImageList* imageList = view->GetImageList();
    if (imageList)
    {
        node->SetImageIndex(imageList->GetImageIndex("document.collection.bitmap"));
    }
    return node;
}

void Content::Clear()
{
    contentDirectories.clear();
    contentFiles.clear();
}

void Content::AddNode(Node* node)
{
    if (node->Kind() == NodeKind::contentDirectory)
    {
        AddContentDirectory(static_cast<ContentDirectory*>(node));
    }
    else if (node->Kind() == NodeKind::contentFile)
    {
        AddContentFile(static_cast<ContentFile*>(node));
    }
}

wing::Control* Content::CreateView(wing::ImageList* imageList)
{
    Clear();
    return CreateContentView(imageList);
}

void Content::AddContentDirectory(ContentDirectory* contentDirectory)
{
    contentDirectory->SetParent(this);
    contentDirectories.push_back(std::unique_ptr<ContentDirectory>(contentDirectory));
}

void Content::AddContentFile(ContentFile* contentFile)
{
    contentFile->SetParent(this);
    contentFiles.push_back(std::unique_ptr<ContentFile>(contentFile));
}

int Content::RuleCount() const
{
    Node* parent = Parent();
    if (parent)
    {
        return parent->RuleCount();
    }
    return 0;
}

Rule* Content::GetRule(int index) const
{
    Node* parent = Parent();
    if (parent)
    {
        return parent->GetRule(index);
    }
    return nullptr;
}

Rule* Content::GetRule(const std::string& name) const
{
    Node* parent = Parent();
    if (parent)
    {
        return parent->GetRule(name);
    }
    return nullptr;
}

Node* Content::GetRuleContainerNode()
{
    Node* parent = Parent();
    if (parent)
    {
        return parent->GetRuleContainerNode();
    }
    return nullptr;
}

ContentDirectory::ContentDirectory(const std::string& name_) : Node(NodeKind::contentDirectory, name_), enabled(true)
{
}

void ContentDirectory::Clear()
{
    contentDirectories.clear();
    contentFiles.clear();
}

wing::Control* ContentDirectory::CreateView(wing::ImageList* imageList)
{
    Clear();
    return CreateContentView(imageList);
}

std::string ContentDirectory::DirectoryPath() const
{
    Node* parent = Parent();
    if (parent)
    {
        return util::Path::Combine(parent->DirectoryPath(), Name());
    }
    else
    {
        return util::Path::Combine("C:/", Name());
    }
}

void ContentDirectory::AddContentDirectory(ContentDirectory* contentDirectory)
{
    contentDirectory->SetParent(this);
    contentDirectories.push_back(std::unique_ptr<ContentDirectory>(contentDirectory));
}

void ContentDirectory::AddContentFile(ContentFile* contentFile)
{
    contentFile->SetParent(this);
    contentFiles.push_back(std::unique_ptr<ContentFile>(contentFile));
}

int ContentDirectory::RuleCount() const
{
    Node* parent = Parent();
    if (parent)
    {
        Rule* rule = parent->GetRule(Name());
        if (rule)
        {
            return rule->RuleCount();
        }
    }
    return 0;
}

Rule* ContentDirectory::GetRule(int index) const
{
    Node* parent = Parent();
    if (parent)
    {
        Rule* rule = parent->GetRule(Name());
        if (rule)
        {
            return rule->GetRule(index);
        }
    }
    return nullptr;
}

Rule* ContentDirectory::GetRule(const std::string& name) const
{
    Node* parent = Parent();
    if (parent)
    {
        Rule* rule = parent->GetRule(Name());
        if (rule)
        {
            return rule->GetRule(name);
        }
    }
    return nullptr;
}

Node* ContentDirectory::GetRuleContainerNode()
{
    Node* parent = Parent();
    if (parent)
    {
        Node* parentRuleContainerNode = parent->GetRuleContainerNode();
        if (parentRuleContainerNode)
        {
            Rule* ruleContainerNode = parentRuleContainerNode->GetRule(Name());
            if (ruleContainerNode)
            {
                return ruleContainerNode;
            }
            else
            {
                ruleContainerNode = new Rule(Name(), RuleKind::include, PathKind::dir);
                parentRuleContainerNode->AddRule(ruleContainerNode);
                Package* package = ruleContainerNode->GetPackage();
                if (package)
                {
                    package->SetDirty();
                }
                return ruleContainerNode;
            }
        }
    }
    return nullptr;
}

void ContentDirectory::AddNode(Node* node)
{
    if (node->Kind() == NodeKind::contentDirectory)
    {
        AddContentDirectory(static_cast<ContentDirectory*>(node));
    }
    else if (node->Kind() == NodeKind::contentFile)
    {
        AddContentFile(static_cast<ContentFile*>(node));
    }
}

bool ContentDirectory::CanOpen() const
{
    return !IsDisabled();
}

bool ContentDirectory::CanExclude() const
{
    return !IsDisabled();
}

bool ContentDirectory::CanInclude() const
{
    return IsDisabled();
}

ContentFile::ContentFile(const std::string& name_) : Node(NodeKind::contentFile, name_), enabled(true)
{
}

bool ContentFile::CanExclude() const
{
    return !IsDisabled();
}

bool ContentFile::CanInclude() const
{
    return IsDisabled();
}

} // package_editor
