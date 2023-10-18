// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.configuration;

import wing;
import dom;
import std.core;

export namespace package_editor {

const int configurationSavePeriod = 3000;
const int configurationSaveTimerId = 1;

class View;

class ColumnWidth
{
public:
    ColumnWidth();
    ColumnWidth(View* view_, const std::string& name_);
    void FromXml(xml::Element* element);
    xml::Element* ToXml() const;
    const std::string& Name() const { return name; }
    bool IsDefined() const { return defined; }
    void SetDefined() { defined = true; }
    int Get() const { return value; }
    void Set(int value_);
private:
    View* view;
    std::string name;
    bool defined;
    int value;
};

class ViewSettings;

class View
{
public:
    View();
    View(ViewSettings* viewSettings_, const std::string& viewName_);
    void FromXml(xml::Element* element);
    xml::Element* ToXml() const;
    const std::string& Name() const { return viewName; }
    void SetChanged();
    ColumnWidth& GetColumnWidth(const std::string& columnName);
private:
    ViewSettings* viewSettings;
    std::string viewName;
    std::map<std::string, ColumnWidth> columnWidthMap;
};

class ViewSettings
{
public:
    ViewSettings();
    ViewSettings(xml::Element* element);
    xml::Element* ToXml() const;
    bool IsChanged() const { return changed; }
    void SetChanged();
    void ResetChanged();
    View& GetView(const std::string& name);
private:
    std::map<std::string, View> viewMap;
    bool changed;
};

void SetWindowState(wing::WindowState windowState);
void SetWindowLocation(const wing::Point& location);
void SetWindowSize(const wing::Size& size);
void SetNormalHorizontalSplitterDistance(int distance);
void SetNormalVerticalSplitterDistance(int distance);
void SetMaximizedHorizontalSplitterDistance(int distance);
void SetMaximizedVerticalSplitterDistance(int distance);
wing::WindowState GetConfiguredWindowState();
wing::Point GetConfiguredWindowLocation();
wing::Size GetConfiguredWindowSize();
int GetConfiguredNormalHorizontalSplitterDistance();
int GetConfiguredNormalVerticalSplitterDistance();
int GetConfiguredMaximizedHorizontalSplitterDistance();
int GetConfiguredMaximizedVerticalSplitterDistance();

ViewSettings& GetConfiguredViewSettings();

bool IsNormalWindowSettingsDefined();
void SetNormalWindowSettingsDefined();
bool IsMaximizedWindowSettingsDefined();
void SetMaximizedWindowSettingsDefined();
bool IsConfigurationChanged();
void LoadConfiguration();
void SaveConfiguration();
void ResetConfiguration();
void InitConfiguration();
void DoneConfiguration();

} // package_editor
