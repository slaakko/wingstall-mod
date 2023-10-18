// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module package_editor.uninstall_component;

import package_editor.error;
import xpath;
import util;

namespace package_editor {

UninstallComponent::UninstallComponent(const std::string& packageXMLFilePath, xml::Element* element)
{
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
                        throw PackageXMLException("uninstall/run' element does not contain a 'command' attribute", packageXMLFilePath, element);
                    }
                }
            }
        }
    }
}

xml::Element* UninstallComponent::ToXml() const
{
    xml::Element* element = xml::MakeElement("uninstall");
    for (const std::string& runCommand : runCommands)
    {
        xml::Element* runElement = xml::MakeElement("run");
        runElement->SetAttribute("command", runCommand);
        element->AppendChild(runElement);
    }
    return element;
}

} // package_editor
