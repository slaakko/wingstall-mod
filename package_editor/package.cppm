// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.package;

import package_editor.preinstall_component;
import package_editor.uninstall_component;
import package_editor.component;
import package_editor.environment;
import package_editor.links;
import package_editor.engine_variable;
import package_editor.node;
import wing_package;
import wing;
import dom;
import util;
import rex;
import std.core;

export namespace package_editor {

class PackageContentView;
class PackageExplorer;

class Properties : public Node
{
public:
    Properties();
    void SetAttributes(xml::Element* packageElement);
    wing::TreeViewNode* ToTreeViewNode(wing::TreeView* view);
    wing::Control* CreateView(wing::ImageList* imageList) override;
    void SetSourceRootDir(const std::string& sourceRootDir_);
    const std::string& SourceRootDir() const { return sourceRootDir; }
    void SetTargetRootDir(const std::string& targetRootDir_);
    const std::string& TargetRootDir() const { return targetRootDir; }
    const std::string& AppName() const { return appName; }
    void SetAppName(const std::string& appName_);
    const std::string& Publisher() const { return publisher; }
    void SetPublisher(const std::string& publisher_);
    const std::string& Version() const { return version; }
    void SetVersion(const std::string& version_);
    wing_package::Compression GetCompression() const { return compression; }
    void SetCompression(wing_package::Compression compression_);
    const std::string& IconFilePath() const { return iconFilePath; }
    void SetIconFilePath(const std::string& iconFilePath_);
    bool IncludeUninstaller() const { return includeUninstaller; }
    void SetIncludeUninstaller(bool includeUninstaller_);
    const util::uuid& Id() const { return id; }
    void SetId(const util::uuid& id_);
    std::string ImageName() const override { return "package.properties.bitmap"; }
private:
    std::string sourceRootDir;
    std::string targetRootDir;
    std::string appName;
    std::string publisher;
    std::string version;
    wing_package::Compression compression;
    std::string iconFilePath;
    bool includeUninstaller;
    util::uuid id;
};

class Package : public Node
{
public:
    Package(const std::string& packageXMLFilePath);
    Package(const std::string& packageXMLFilePath, xml::Element* root);
    bool CanSave() const override;
    void Save() override;
    bool CanBuild() const override;
    void Build() override;
    bool CanClose() const override { return true; }
    void Close() override;
    bool IsDirty() const { return dirty; }
    void SetDirty();
    bool Building() const { return building; }
    void ResetBuilding();
    xml::Element* ToXml() const;
    std::string PathComponentName() const override { return "Package"; }
    std::string ViewName() const override { return "package_view"; }
    const std::string& FilePath() const { return filePath; }
    std::string BinDirectoryPath() const;
    Properties* GetProperties() const { return properties.get(); }
    Components* GetComponents() const { return components.get(); }
    Environment* GetEnvironment() const { return environment.get(); }
    Links* GetLinks() const { return links.get(); }
    EngineVariables* GetEngineVariables() const { return engineVariables.get(); }
    wing::TreeViewNode* ToTreeViewNode(wing::TreeView* view);
    wing::Control* CreateView(wing::ImageList* imageList)  override;
    Package* GetPackage() const override { return const_cast<Package*>(this); }
    PackageContentView* ContentView() const { return view; }
    void SetContentView(PackageContentView* view_) { view = view_; }
    PackageExplorer* Explorer() const { return explorer; }
    void SetExplorer(PackageExplorer* explorer_) { explorer = explorer_; }
    rex::Context& GetContext() { return context; }
private:
    std::string filePath;
    std::unique_ptr<Properties> properties;
    std::unique_ptr<PreinstallComponent> preinstallComponent;
    std::unique_ptr<Components> components;
    std::unique_ptr<Environment> environment;
    std::unique_ptr<Links> links;
    std::unique_ptr<UninstallComponent> uninstallComponent;
    std::unique_ptr<EngineVariables> engineVariables;
    PackageContentView* view;
    PackageExplorer* explorer;
    rex::Context context;
    bool dirty;
    bool building;
};

} // package_editor
