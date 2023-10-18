// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing_package.variable;

import wing_package.package;
import wing_package.links;
import wing_package.environment;
import wing_package.file;
import wing;
import util;

namespace wing_package {

Variable::Variable() : Node(NodeKind::variable)
{
}

Variable::Variable(const std::string& name_) : Node(NodeKind::variable, name_)
{
}

std::string Variable::Value() const
{
    return std::string();
}

xml::Element* Variable::ToXml() const
{
    xml::Element* element = xml::MakeElement("variable");
    element->SetAttribute("name", Name());
    element->SetAttribute("value", Value());
    return element;
}

TargetRootDirVariable::TargetRootDirVariable() : Variable("TARGET_ROOT_DIR")
{
}

std::string TargetRootDirVariable::Value() const
{
    const Package* package = GetPackage();
    if (package)
    {
        return package->TargetRootDir();
    }
    else
    {
        return Variable::Value();
    }
}

PreinstallDirVariable::PreinstallDirVariable() : Variable("PREINSTALL_DIR")
{
}

std::string PreinstallDirVariable::Value() const
{
    Package* package = GetPackage();
    if (package)
    {
        return package->PreinstallDir();
    }
    else
    {
        return Variable::Value();
    }
}

AppNameVariable::AppNameVariable() : Variable("APP_NAME")
{
}

std::string AppNameVariable::Value() const
{
    Package* package = GetPackage();
    if (package)
    {
        return package->AppName();
    }
    else
    {
        return Variable::Value();
    }
}

AppVersionVariable::AppVersionVariable() : Variable("APP_VERSION")
{
}

std::string AppVersionVariable::Value() const
{
    Package* package = GetPackage();
    if (package)
    {
        return package->Version();
    }
    else
    {
        return Variable::Value();
    }
}

ProductIdVariable::ProductIdVariable() : Variable("PRODUCT_ID")
{
}

std::string ProductIdVariable::Value() const
{
    Package* package = GetPackage();
    if (package)
    {
        return util::ToString(package->Id());
    }
    else
    {
        return Variable::Value();
    }
}

PublisherVariable::PublisherVariable() : Variable("PUBLISHER")
{
}

std::string PublisherVariable::Value() const
{
    Package* package = GetPackage();
    if (package)
    {
        return package->Publisher();
    }
    else
    {
        return Variable::Value();
    }
}

StartMenuProgramsFolderVariable::StartMenuProgramsFolderVariable() : Variable("START_MENU_PROGRAMS_FOLDER")
{
}

std::string StartMenuProgramsFolderVariable::Value() const
{
    try
    {
        return wing::GetStartMenuProgramsFolderPath();
    }
    catch (const std::exception& ex)
    {
        throw std::runtime_error("could not get start menu programs folder path: " + std::string(ex.what()));
    }
}

DesktopFolderVariable::DesktopFolderVariable() : Variable("DESKTOP_FOLDER")
{
}

std::string DesktopFolderVariable::Value() const
{
    try
    {
        return wing::GetDesktopFolderPath();
    }
    catch (const std::exception& ex)
    {
        throw std::runtime_error("could not get desktop folder path: " + std::string(ex.what()));
    }
}

ProgramFilesDirVariable::ProgramFilesDirVariable() : Variable("PROGRAM_FILES_DIR")
{
}

std::string ProgramFilesDirVariable::Value() const
{
    try
    {
        return wing::GetProgramFilesDirectoryPath();
    }
    catch (const std::exception& ex)
    {
        throw std::runtime_error("could not get program files folder path: " + std::string(ex.what()));
    }
}

Variables::Variables() : Node(NodeKind::variables, "variables")
{
    AddVariable(new TargetRootDirVariable());
    AddVariable(new PreinstallDirVariable());
    AddVariable(new AppNameVariable());
    AddVariable(new AppVersionVariable());
    AddVariable(new ProductIdVariable());
    AddVariable(new PublisherVariable());
    AddVariable(new StartMenuProgramsFolderVariable());
    AddVariable(new DesktopFolderVariable());
    AddVariable(new ProgramFilesDirVariable());
}

void Variables::AddVariable(Variable* variable)
{
    variable->SetParent(this);
    variables.push_back(std::unique_ptr<Variable>(variable));
    variableMap[variable->Name()] = variable;
}

Variable* Variables::GetVariable(const std::string& name) const
{
    auto it = variableMap.find(name);
    if (it != variableMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

xml::Element* Variables::ToXml() const
{
    xml::Element* element = xml::MakeElement("variables");
    for (const auto& variable : variables)
    {
        xml::Element* child = variable->ToXml();
        element->AppendChild(child);
    }
    return element;
}

std::string Variables::ExpandPath(const std::string& path) const
{
    std::string result;
    std::string variableName;
    int state = 0;
    for (char c : path)
    {
        switch (state)
        {
        case 0:
        {
            if (c == '$')
            {
                variableName.clear();
                state = 1;
            }
            else
            {
                result.append(1, c);
            }
            break;
        }
        case 1:
        {
            if (c == '$')
            {
                Variable* variable = GetVariable(variableName);
                if (variable)
                {
                    result.append(variable->Value());
                }
                else
                {
                    throw std::runtime_error("installation variable '" + variableName + "' not found");
                }
                state = 0;
            }
            else
            {
                variableName.append(1, c);
            }
            break;
        }
        }
    }
    if (state == 1)
    {
        throw std::runtime_error("installation variable '" + variableName + "' has no ending '$' character");
    }
    return util::MakeNativePath(result);
}

} // namespace wing_package
