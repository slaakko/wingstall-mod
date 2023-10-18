// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.resource_manager;

import std.core;

export namespace wing {

class Cursor;
class Icon;

class Resource
{
public:
    virtual ~Resource();
    virtual bool IsCursor() const { return false; }
    virtual bool IsIcon() const { return false; }
};

class ResourceManager
{
public:
    ResourceManager();
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;
    Resource* GetResource(const std::string& resourceName) const;
    Cursor& GetCursor(const std::string& cursorName);
    Icon& GetIcon(const std::string& iconName);
private:
    std::unordered_map<std::string, Resource*> resourceMap;
    std::vector<std::unique_ptr<Resource>> resources;
};

} // wing
