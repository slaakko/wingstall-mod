// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.engine_variable;

import package_editor.node;
import wing;
import std.core;

export namespace package_editor {

class EngineVariable;

class EngineVariables : public Node
{
public:
    EngineVariables();
    void Fetch();
    wing::TreeViewNode* ToTreeViewNode(wing::TreeView* view);
    wing::Control* CreateView(wing::ImageList* imageList) override;
    std::string ImageName() const override { return "engine.variables.bitmap"; }
    std::string ViewName() const override { return "engine_variables_view"; }
private:
    std::vector<std::unique_ptr<EngineVariable>> engineVariables;
};

class EngineVariable : public Node
{
public:
    EngineVariable();
    wing::TreeViewNode* ToTreeViewNode(wing::TreeView* view);
    std::string ImageName() const override { return "engine.variable.bitmap"; }
    void SetData(wing::ListViewItem* item, wing::ImageList* imageList) override;
    virtual std::string Value() const = 0;
    bool Leaf() const override { return true; }
    bool CanOpen() const override { return false; }
    void ExecuteDefaultAction() override { Parent()->Open(); }
};

class TargetRootDirVariable : public EngineVariable
{
public:
    TargetRootDirVariable();
    std::string Value() const override;
};

class AppNameVariable : public EngineVariable
{
public:
    AppNameVariable();
    std::string Value() const override;
};

class AppVersionVariable : public EngineVariable
{
public:
    AppVersionVariable();
    std::string Value() const override;
};

class ProductIdVariable : public EngineVariable
{
public:
    ProductIdVariable();
    std::string Value() const override;
};

class PublisherVariable : public EngineVariable
{
public:
    PublisherVariable();
    std::string Value() const override;
};

class StartMenuProgramsFolderVariable : public EngineVariable
{
public:
    StartMenuProgramsFolderVariable();
    std::string Value() const override;
};

class DesktopFolderVariable : public EngineVariable
{
public:
    DesktopFolderVariable();
    std::string Value() const override;
};

class ProgramFilesDirVariable : public EngineVariable
{
public:
    ProgramFilesDirVariable();
    std::string Value() const override;
};

} // package_editor
