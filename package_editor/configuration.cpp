// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module package_editor.configuration;

import wingstall_config;
import xpath;
import util;
import std.filesystem;

namespace package_editor {

ColumnWidth::ColumnWidth() : view(), name(), defined(false), value(0)
{
}

ColumnWidth::ColumnWidth(View* view_, const std::string& name_) : view(view_), name(name_), defined(false), value(0)
{
}

void ColumnWidth::FromXml(xml::Element* element)
{
    std::string widthAttr = element->GetAttribute("width");
    if (!widthAttr.empty())
    {
        value = std::stoi(widthAttr);
    }
    std::string definedAttr = element->GetAttribute("defined");
    if (!definedAttr.empty())
    {
        if (definedAttr == "true")
        {
            defined = true;
        }
        else if (definedAttr == "false")
        {
            defined = false;
        }
    }
}

xml::Element* ColumnWidth::ToXml() const
{
    xml::Element* element = xml::MakeElement("column");
    element->SetAttribute("name", name);
    element->SetAttribute("width", std::to_string(value));
    element->SetAttribute("defined", defined ? "true" : "false");
    return element;
}

void ColumnWidth::Set(int value_)
{
    if (value != value_)
    {
        value = value_;
        defined = true;
        view->SetChanged();
    }
}

View::View() : viewSettings(), viewName()
{
}

View::View(ViewSettings* viewSettings_, const std::string& viewName_) : viewSettings(viewSettings_), viewName(viewName_)
{
}

void View::FromXml(xml::Element* element)
{
    std::unique_ptr<xpath::XPathObject> columnWidthObject = xpath::Evaluate("column", element);
    if (columnWidthObject)
    {
        if (columnWidthObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(columnWidthObject.get());
            int n = nodeSet->Length();
            for (int i = 0; i < n; ++i)
            {
                xml::Node* node = (*nodeSet)[i];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    std::string nameAttr = element->GetAttribute("name");
                    if (!nameAttr.empty())
                    {
                        ColumnWidth& columnWidth = GetColumnWidth(nameAttr);
                        columnWidth.FromXml(element);
                    }
                }
            }
        }
    }
}

xml::Element* View::ToXml() const
{
    xml::Element* element = xml::MakeElement("view");
    element->SetAttribute("name", viewName);
    for (const auto& p : columnWidthMap)
    {
        element->AppendChild(p.second.ToXml());
    }
    return element;
}

void View::SetChanged()
{
    viewSettings->SetChanged();
}

ColumnWidth& View::GetColumnWidth(const std::string& columnName)
{
    auto it = columnWidthMap.find(columnName);
    if (it == columnWidthMap.cend())
    {
        columnWidthMap.insert(std::make_pair(columnName, ColumnWidth(this, columnName)));
    }
    return columnWidthMap[columnName];
}

ViewSettings::ViewSettings() : changed(false)
{
}

ViewSettings::ViewSettings(xml::Element* element) : changed(false)
{
    std::unique_ptr<xpath::XPathObject> viewObject = xpath::Evaluate("view", element);
    if (viewObject)
    {
        if (viewObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(viewObject.get());
            int n = nodeSet->Length();
            for (int i = 0; i < n; ++i)
            {
                xml::Node* node = (*nodeSet)[i];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    std::string nameAttr = element->GetAttribute("name");
                    if (!nameAttr.empty())
                    {
                        View& view = GetView(nameAttr);
                        view.FromXml(element);
                    }
                }
            }
        }
    }
}

xml::Element* ViewSettings::ToXml() const
{
    xml::Element* element = xml::MakeElement("viewSettings");
    for (const auto& p : viewMap)
    {
        element->AppendChild(p.second.ToXml());
    }
    return element;
}

void ViewSettings::SetChanged()
{
    changed = true;
}

void ViewSettings::ResetChanged()
{
    changed = false;
}

View& ViewSettings::GetView(const std::string& name)
{
    auto it = viewMap.find(name);
    if (it == viewMap.cend())
    {
        viewMap.insert(std::make_pair(name, View(this, name)));
    }
    return viewMap[name];
}

class WindowLocation
{
public:
    WindowLocation();
    WindowLocation(xml::Element* element);
    xml::Element* ToXml() const;
    const wing::Point& Loc() const { return loc; }
    void SetLoc(const wing::Point& loc_, bool& changed);
private:
    wing::Point loc;
};

xml::Element* WindowLocation::ToXml() const
{
    xml::Element* element = xml::MakeElement("location");
    element->SetAttribute("x", std::to_string(loc.X));
    element->SetAttribute("y", std::to_string(loc.Y));
    return element;
}

void WindowLocation::SetLoc(const wing::Point& loc_, bool& changed)
{
    if (loc.X != loc_.X || loc.Y != loc_.Y)
    {
        loc = loc_;
        changed = true;
    }
}

WindowLocation::WindowLocation() : loc()
{
}

WindowLocation::WindowLocation(xml::Element* element)
{
    std::string xAttr = element->GetAttribute("x");
    if (!xAttr.empty())
    {
        loc.X = std::stoi(xAttr);
    }
    std::string yAttr = element->GetAttribute("y");
    if (!yAttr.empty())
    {
        loc.Y = std::stoi(yAttr);
    }
}

class WindowSize
{
public:
    WindowSize();
    WindowSize(xml::Element* element);
    xml::Element* ToXml() const;
    const wing::Size& Sz() const { return sz; }
    void SetSz(const wing::Size& sz_, bool& changed);
private:
    wing::Size sz;
};

WindowSize::WindowSize() : sz()
{
}

WindowSize::WindowSize(xml::Element* element)
{
    std::string widthAttr = element->GetAttribute("width");
    if (!widthAttr.empty())
    {
        sz.Width = std::stoi(widthAttr);
    }
    std::string heightAttr = element->GetAttribute("height");
    if (!heightAttr.empty())
    {
        sz.Height = std::stoi(heightAttr);
    }
}

xml::Element* WindowSize::ToXml() const
{
    xml::Element* element = xml::MakeElement("size");
    element->SetAttribute("width", std::to_string(sz.Width));
    element->SetAttribute("height", std::to_string(sz.Height));
    return element;
}

void WindowSize::SetSz(const wing::Size& sz_, bool& changed)
{
    if (sz.Width != sz_.Width || sz.Height != sz_.Height)
    {
        sz = sz_;
        changed = true;
    }
}

class WindowSettings
{
public:
    WindowSettings();
    WindowSettings(xml::Element* element);
    xml::Element* ToXml() const;
    bool IsNormalDefined() const { return normalDefined; }
    void SetNormalDefined() { normalDefined = true; }
    bool IsMaximizedDefined() const { return maximizedDefined; }
    void SetMaximizedDefined() { maximizedDefined = true; }
    bool IsChanged() const { return changed; }
    void ResetChanged();
    wing::WindowState GetWindowState() const { return windowState; }
    void SetWindowState(wing::WindowState windowState_);
    const wing::Point& GetWindowLocation() const { return windowLocation.Loc(); }
    void SetWindowLocation(const wing::Point& loc_);
    const wing::Size& GetWindowSize() const { return windowSize.Sz(); }
    void SetWindowSize(const wing::Size& sz_);
    int MaximizedHorizontalSplitterDistance() { return maximizedHorizontalSplitterDistance; }
    void SetMaximizedHorizontalSplitterDistance(int distance);
    int MaximizedVerticalSplitterDistance() const { return maximizedVerticalSplitterDistance; }
    void SetMaximizedVerticalSplitterDistance(int distance);
    int NormalHorizontalSplitterDistance() const { return normalHorizontalSplitterDistance; }
    void SetNormalHorizontalSplitterDistance(int distance);
    int NormalVerticalSplitterDistance() const { return normalVerticalSplitterDistance; }
    void SetNormalVerticalSplitterDistance(int distance);
private:
    bool normalDefined;
    bool maximizedDefined;
    bool changed;
    wing::WindowState windowState;
    WindowLocation windowLocation;
    WindowSize windowSize;
    int maximizedHorizontalSplitterDistance;
    int maximizedVerticalSplitterDistance;
    int normalHorizontalSplitterDistance;
    int normalVerticalSplitterDistance;
};

WindowSettings::WindowSettings() :
    normalDefined(false),
    maximizedDefined(false),
    changed(false),
    windowState(wing::WindowState::normal),
    windowLocation(),
    windowSize(),
    maximizedHorizontalSplitterDistance(0),
    maximizedVerticalSplitterDistance(0),
    normalHorizontalSplitterDistance(0),
    normalVerticalSplitterDistance(0)
{
}

WindowSettings::WindowSettings(xml::Element* element) :
    normalDefined(false),
    maximizedDefined(false),
    changed(false),
    windowState(wing::WindowState::normal),
    windowLocation(),
    windowSize(),
    maximizedHorizontalSplitterDistance(0),
    maximizedVerticalSplitterDistance(0),
    normalHorizontalSplitterDistance(0),
    normalVerticalSplitterDistance(0)
{
    std::string normalDefinedAttr = element->GetAttribute("normalDefined");
    if (normalDefinedAttr == "true")
    {
        normalDefined = true;
    }
    else if (normalDefinedAttr == "false")
    {
        normalDefined = false;
    }
    else
    {
        throw std::runtime_error("invalid configuration, normalDefined attribute not found");
    }
    std::string maximizedDefinedAttr = element->GetAttribute("maximizedDefined");
    if (maximizedDefinedAttr == "true")
    {
        maximizedDefined = true;
    }
    else if (maximizedDefinedAttr == "false")
    {
        maximizedDefined = false;
    }
    else
    {
        throw std::runtime_error("invalid configuration, maximizedDefined attribute not found");
    }
    std::string windowStateAttr = element->GetAttribute("windowState");
    if (!windowStateAttr.empty())
    {
        windowState = static_cast<wing::WindowState>(std::stoi(windowStateAttr));
    }
    std::string maximizedHorizontalSplitterDistanceAttr = element->GetAttribute("maximizedHorizontalSplitterDistance");
    if (!maximizedHorizontalSplitterDistanceAttr.empty())
    {
        maximizedHorizontalSplitterDistance = std::stoi(maximizedHorizontalSplitterDistanceAttr);
    }
    std::string maximizedVerticalSplitterDistanceAttr = element->GetAttribute("maximizedVerticalSplitterDistance");
    if (!maximizedVerticalSplitterDistanceAttr.empty())
    {
        maximizedVerticalSplitterDistance = std::stoi(maximizedVerticalSplitterDistanceAttr);
    }
    std::string normalHorizontalSplitterDistanceAttr = element->GetAttribute("normalHorizontalSplitterDistance");
    if (!normalHorizontalSplitterDistanceAttr.empty())
    {
        normalHorizontalSplitterDistance = std::stoi(normalHorizontalSplitterDistanceAttr);
    }
    std::string normalVerticalSplitterDistanceAttr = element->GetAttribute("normalVerticalSplitterDistance");
    if (!normalVerticalSplitterDistanceAttr.empty())
    {
        normalVerticalSplitterDistance = std::stoi(normalVerticalSplitterDistanceAttr);
    }
    std::unique_ptr<xpath::XPathObject> windowLocationObject = xpath::Evaluate("location", element);
    if (windowLocationObject)
    {
        if (windowLocationObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(windowLocationObject.get());
            int n = nodeSet->Length();
            if (n == 1)
            {
                xml::Node* node = (*nodeSet)[0];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    windowLocation = WindowLocation(element);
                }
            }
            else
            {
                throw std::runtime_error("invalid configuration, single /configuration/windowSettings/location element expected");
            }
        }
    }
    std::unique_ptr<xpath::XPathObject> windowSizeObject = xpath::Evaluate("size", element);
    if (windowSizeObject)
    {
        if (windowSizeObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(windowSizeObject.get());
            int n = nodeSet->Length();
            if (n == 1)
            {
                xml::Node* node = (*nodeSet)[0];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    windowSize = WindowSize(element);
                }
            }
            else
            {
                throw std::runtime_error("invalid configuration, single /configuration/windowSettings/size element expected");
            }
        }
    }
}

xml::Element* WindowSettings::ToXml() const
{
    xml::Element* element = xml::MakeElement("windowSettings");
    element->SetAttribute("normalDefined", normalDefined ? "true" : "false");
    element->SetAttribute("maximizedDefined", maximizedDefined ? "true" : "false");
    element->SetAttribute("windowState", std::to_string(static_cast<int>(windowState)));
    element->SetAttribute("maximizedHorizontalSplitterDistance", std::to_string(maximizedHorizontalSplitterDistance));
    element->SetAttribute("maximizedVerticalSplitterDistance", std::to_string(maximizedVerticalSplitterDistance));
    element->SetAttribute("normalHorizontalSplitterDistance", std::to_string(normalHorizontalSplitterDistance));
    element->SetAttribute("normalVerticalSplitterDistance", std::to_string(normalVerticalSplitterDistance));
    element->AppendChild(windowLocation.ToXml());
    element->AppendChild(windowSize.ToXml());
    return element;
}

void WindowSettings::ResetChanged()
{
    changed = false;
}

void WindowSettings::SetWindowState(wing::WindowState windowState_)
{
    if (windowState != windowState_)
    {
        windowState = windowState_;
        changed = true;
    }
}

void WindowSettings::SetWindowLocation(const wing::Point& loc_)
{
    windowLocation.SetLoc(loc_, changed);
}

void WindowSettings::SetWindowSize(const wing::Size& sz_)
{
    windowSize.SetSz(sz_, changed);
}

void WindowSettings::SetMaximizedHorizontalSplitterDistance(int distance)
{
    if (maximizedHorizontalSplitterDistance != distance)
    {
        maximizedHorizontalSplitterDistance = distance;
        changed = true;
    }
}

void WindowSettings::SetMaximizedVerticalSplitterDistance(int distance)
{
    if (maximizedVerticalSplitterDistance != distance)
    {
        maximizedVerticalSplitterDistance = distance;
        changed = true;
    }
}

void WindowSettings::SetNormalHorizontalSplitterDistance(int distance)
{
    if (normalHorizontalSplitterDistance != distance)
    {
        normalHorizontalSplitterDistance = distance;
        changed = true;
    }
}

void WindowSettings::SetNormalVerticalSplitterDistance(int distance)
{
    if (normalVerticalSplitterDistance != distance)
    {
        normalVerticalSplitterDistance = distance;
        changed = true;
    }
}

class Configuration
{
public:
    Configuration();
    Configuration(xml::Document* document);
    xml::Element* ToXml() const;
    WindowSettings& GetWindowSettings() { return *windowSettings; }
    ViewSettings& GetViewSettings() { return *viewSettings; }
private:
    std::unique_ptr<WindowSettings> windowSettings;
    std::unique_ptr<ViewSettings> viewSettings;
};

Configuration::Configuration() : windowSettings(new WindowSettings()), viewSettings(new ViewSettings())
{
}

Configuration::Configuration(xml::Document* document)
{
    std::unique_ptr<xpath::XPathObject> windowSettingsObject = xpath::Evaluate("/configuration/windowSettings", document);
    if (windowSettingsObject)
    {
        if (windowSettingsObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(windowSettingsObject.get());
            int n = nodeSet->Length();
            if (n == 1)
            {
                xml::Node* node = (*nodeSet)[0];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    windowSettings.reset(new WindowSettings(element));
                }
            }
            else
            {
                throw std::runtime_error("invalid configuration, single /configuration/windowSettings element expected");
            }
        }
    }
    std::unique_ptr<xpath::XPathObject> viewSettingsObject = xpath::Evaluate("/configuration/viewSettings", document);
    if (viewSettingsObject)
    {
        xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(viewSettingsObject.get());
        int n = nodeSet->Length();
        if (n == 1)
        {
            xml::Node* node = (*nodeSet)[0];
            if (node->Kind() == xml::NodeKind::elementNode)
            {
                xml::Element* element = static_cast<xml::Element*>(node);
                viewSettings.reset(new ViewSettings(element));
            }
        }
        else
        {
            throw std::runtime_error("invalid configuration, single /configuration/viewSettings element expected");
        }
    }
}

xml::Element* Configuration::ToXml() const
{
    xml::Element* element = xml::MakeElement("configuration");
    element->AppendChild(windowSettings->ToXml());
    element->AppendChild(viewSettings->ToXml());
    return element;
}

class Config
{
public:
    static void Init();
    static void Done();
    static Config& Instance() { return *instance; }
    Configuration& GetConfiguration() { return *configuration; }
    void SetConfiguration(Configuration* configuration_);
    const std::string& FilePath() const { return filePath; }
private:
    Config();
    static std::unique_ptr<Config> instance;
    std::unique_ptr<Configuration> configuration;
    std::string filePath;
};

std::unique_ptr<Config> Config::instance;

Config::Config() : configuration(new Configuration()), filePath(util::Path::Combine(util::GetFullPath(wingstall_config::WingstallConfigDir()), "package_editor.config.xml"))
{
}

void Config::SetConfiguration(Configuration* configuration_)
{
    configuration.reset(configuration_);
}

void Config::Init()
{
    instance.reset(new Config());
}

void Config::Done()
{
    instance.reset();
}

ViewSettings& GetConfiguredViewSettings()
{
    return Config::Instance().GetConfiguration().GetViewSettings();
}

bool IsNormalWindowSettingsDefined()
{
    return Config::Instance().GetConfiguration().GetWindowSettings().IsNormalDefined();
}

void SetNormalWindowSettingsDefined()
{
    Config::Instance().GetConfiguration().GetWindowSettings().SetNormalDefined();
}

bool IsMaximizedWindowSettingsDefined()
{
    return Config::Instance().GetConfiguration().GetWindowSettings().IsMaximizedDefined();
}

void SetMaximizedWindowSettingsDefined()
{
    Config::Instance().GetConfiguration().GetWindowSettings().SetMaximizedDefined();
}

void SetWindowState(wing::WindowState windowState)
{
    Config::Instance().GetConfiguration().GetWindowSettings().SetWindowState(windowState);
}

void SetWindowLocation(const wing::Point& location)
{
    Config::Instance().GetConfiguration().GetWindowSettings().SetWindowLocation(location);
}

void SetWindowSize(const wing::Size& size)
{
    Config::Instance().GetConfiguration().GetWindowSettings().SetWindowSize(size);
}

void SetNormalHorizontalSplitterDistance(int distance)
{
    Config::Instance().GetConfiguration().GetWindowSettings().SetNormalHorizontalSplitterDistance(distance);
}

void SetNormalVerticalSplitterDistance(int distance)
{
    Config::Instance().GetConfiguration().GetWindowSettings().SetNormalVerticalSplitterDistance(distance);
}

void SetMaximizedHorizontalSplitterDistance(int distance)
{
    Config::Instance().GetConfiguration().GetWindowSettings().SetMaximizedHorizontalSplitterDistance(distance);
}

void SetMaximizedVerticalSplitterDistance(int distance)
{
    Config::Instance().GetConfiguration().GetWindowSettings().SetMaximizedVerticalSplitterDistance(distance);
}

wing::WindowState GetConfiguredWindowState()
{
    return Config::Instance().GetConfiguration().GetWindowSettings().GetWindowState();
}

wing::Point GetConfiguredWindowLocation()
{
    return Config::Instance().GetConfiguration().GetWindowSettings().GetWindowLocation();
}

wing::Size GetConfiguredWindowSize()
{
    return Config::Instance().GetConfiguration().GetWindowSettings().GetWindowSize();
}

int GetConfiguredNormalHorizontalSplitterDistance()
{
    return Config::Instance().GetConfiguration().GetWindowSettings().NormalHorizontalSplitterDistance();
}

int GetConfiguredNormalVerticalSplitterDistance()
{
    return Config::Instance().GetConfiguration().GetWindowSettings().NormalVerticalSplitterDistance();
}

int GetConfiguredMaximizedHorizontalSplitterDistance()
{
    return Config::Instance().GetConfiguration().GetWindowSettings().MaximizedHorizontalSplitterDistance();
}

int GetConfiguredMaximizedVerticalSplitterDistance()
{
    return Config::Instance().GetConfiguration().GetWindowSettings().MaximizedVerticalSplitterDistance();
}

bool IsConfigurationChanged()
{
    if (Config::Instance().GetConfiguration().GetWindowSettings().IsChanged())
    {
        return true;
    }
    if (Config::Instance().GetConfiguration().GetViewSettings().IsChanged())
    {
        return true;
    }
    return false;
}

void LoadConfiguration()
{
    if (std::filesystem::exists(Config::Instance().FilePath()))
    {
        std::unique_ptr<xml::Document> configurationDoc = xml::ReadDocument(Config::Instance().FilePath());
        Config::Instance().SetConfiguration(new Configuration(configurationDoc.get()));
    }
}

void SaveConfiguration()
{
    xml::Document configurationDoc;
    configurationDoc.AppendChild(Config::Instance().GetConfiguration().ToXml());
    std::ofstream configurationFile(Config::Instance().FilePath());
    util::CodeFormatter formatter(configurationFile);
    formatter.SetIndentSize(1);
    configurationDoc.Write(formatter);
    Config::Instance().GetConfiguration().GetWindowSettings().ResetChanged();
    Config::Instance().GetConfiguration().GetViewSettings().ResetChanged();
}

void ResetConfiguration()
{
    InitConfiguration();
    if (std::filesystem::exists(Config::Instance().FilePath()))
    {
        std::error_code ec;
        std::filesystem::remove(Config::Instance().FilePath(), ec);
        if (ec)
        {
            throw std::runtime_error("could not remove '" + Config::Instance().FilePath() + "': " + util::PlatformStringToUtf8(ec.message()));
        }
    }
}

void InitConfiguration()
{
    Config::Init();
}

void DoneConfiguration()
{
    Config::Done();
}

} // package_editor
