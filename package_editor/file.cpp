// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module package_editor.file;

import package_editor.error;
import util;

namespace package_editor {

File::File(const std::string& name_) : Node(NodeKind::file, name_)
{
}

File::File(const std::string& packageXMLFilePath, xml::Element* element) : Node(NodeKind::file, std::string())
{
    std::string nameAttr = element->GetAttribute("name");
    if (!nameAttr.empty())
    {
        SetName(nameAttr);
    }
    else
    {
        throw PackageXMLException("'file' element has no 'name' attribute", packageXMLFilePath, element);
    }
}

xml::Element* File::ToXml() const
{
    xml::Element* element = xml::MakeElement("file");
    element->SetAttribute("name", Name());
    return element;
}

wing::TreeViewNode* File::ToTreeViewNode(wing::TreeView* view)
{
    wing::TreeViewNode* node = new wing::TreeViewNode(Name());
    SetTreeViewNode(node);
    node->SetData(this);
    wing::ImageList* imageList = view->GetImageList();
    if (imageList)
    {
        node->SetImageIndex(imageList->GetImageIndex("file.bitmap"));
    }
    return node;
}

} // package_editor
