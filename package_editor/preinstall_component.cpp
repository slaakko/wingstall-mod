// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module package_editor.preinstall_component;

import package_editor.error;
import xpath;
import util;

namespace package_editor {

PreinstallComponent::PreinstallComponent(const std::string& packageXMLFilePath, xml::Element* element)
{
    std::unique_ptr<xpath::XPathObject> includeObject = xpath::Evaluate("include", element);
    if (includeObject)
    {
        if (includeObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(includeObject.get());
            int n = nodeSet->Length();
            for (int i = 0; i < n; ++i)
            {
                xml::Node* node = (*nodeSet)[i];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    std::string filePathAttr = element->GetAttribute("filePath");
                    if (!filePathAttr.empty())
                    {
                        includeFilePaths.push_back(filePathAttr);
                    }
                    else
                    {
                        throw PackageXMLException("preinstall/include' element does not contain a 'filePath' attribute", packageXMLFilePath, element);
                    }
                }
            }
        }
    }
    std::unique_ptr<xpath::XPathObject> runObject = xpath::Evaluate("run", element);
    if (runObject)
    {
        if (runObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(runObject.get());
            int n = nodeSet->Length();
            for (int i = 0; i < n; ++i)
            {
                xml::Node* node = (*nodeSet)[i];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    std::string commandAttr = element->GetAttribute("command");
                    if (!commandAttr.empty())
                    {
                        runCommands.push_back(commandAttr);
                    }
                    else
                    {
                        throw PackageXMLException("preinstall/run' element does not contain a 'command' attribute", packageXMLFilePath, element);
                    }
                }
            }
        }
    }
}

xml::Element* PreinstallComponent::ToXml() const
{
    xml::Element* element = xml::MakeElement("preinstall");
    for (const std::string& includeFilePath : includeFilePaths)
    {
        xml::Element* includeElement = xml::MakeElement("include");
        includeElement->SetAttribute("filePath", includeFilePath);
        element->AppendChild(includeElement);
    }
    for (const std::string& runCommand : runCommands)
    {
        xml::Element* runElement = xml::MakeElement("run");
        runElement->SetAttribute("command", runCommand);
        element->AppendChild(runElement);
    }
    return element;
}

} // package_editor
