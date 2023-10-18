// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

export module wing.icon;

import wing.resource_manager;
import std.core;

export namespace wing {

class Icon : public Resource
{
public:
    Icon();
    Icon(HICON handle_);
    Icon(HICON handle_, bool standard_);
    Icon(const Icon&) = delete;
    Icon(Icon&& that) noexcept;
    Icon& operator=(const Icon&) = delete;
    Icon& operator=(Icon&& that) noexcept;
    ~Icon();
    bool IsIcon() const override { return true; }
    HICON Handle() const { return handle; }
    bool Standard() const { return standard; }
private:
    HICON handle;
    bool standard;
};

Icon LoadStandardIcon(int standardIconId);
Icon LoadIconByName(const std::string& iconName);

} // wing
