// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing_package.variable;

import wing_package.node;
import dom;
import std.core;

export namespace wing_package {

class Variable : public Node
{
public:
    Variable();
    Variable(const std::string& name_);
    virtual std::string Value() const;
    xml::Element* ToXml() const override;
private:
    std::string name;
};

class TargetRootDirVariable : public Variable
{
public:
    TargetRootDirVariable();
    std::string Value() const override;
};

class PreinstallDirVariable : public Variable
{
public:
    PreinstallDirVariable();
    std::string Value() const override;
};

class AppNameVariable : public Variable
{
public:
    AppNameVariable();
    std::string Value() const override;
};

class AppVersionVariable : public Variable
{
public:
    AppVersionVariable();
    std::string Value() const override;
};

class ProductIdVariable : public Variable
{
public:
    ProductIdVariable();
    std::string Value() const override;
};

class PublisherVariable : public Variable
{
public:
    PublisherVariable();
    std::string Value() const override;
};

class StartMenuProgramsFolderVariable : public Variable
{
public:
    StartMenuProgramsFolderVariable();
    std::string Value() const override;
};

class DesktopFolderVariable : public Variable
{
public:
    DesktopFolderVariable();
    std::string Value() const override;
};

class ProgramFilesDirVariable : public Variable
{
public:
    ProgramFilesDirVariable();
    std::string Value() const override;
};

class Variables : public Node
{
public:
    Variables();
    void AddVariable(Variable* variable);
    Variable* GetVariable(const std::string& name) const;
    std::string ExpandPath(const std::string& path) const;
    xml::Element* ToXml() const override;
private:
    std::map<std::string, Variable*> variableMap;
    std::vector<std::unique_ptr<Variable>> variables;
};

} // namespace wing_package
