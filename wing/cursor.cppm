// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

export module wing.cursor;

import wing.resource_manager;
import std.core;

export namespace wing {

enum class StandardCursorId : int
{
    appStarting = 32650,
    arrow = 32512,
    cross = 32515,
    hand = 32649,
    help = 32651,
    ibeam = 32513,
    no = 32648,
    sizeAll = 32646,
    sizeNewSW = 32643,
    sizeNS = 32645,
    sizeNWSE = 32642,
    sizeWE = 32644,
    upArrow = 32516,
    wait = 32514
};

class Cursor : public Resource
{
public:
    Cursor();
    Cursor(HCURSOR handle_);
    Cursor(HCURSOR handle_, bool standard_);
    Cursor(const Cursor&) = delete;
    Cursor(Cursor&& that) noexcept;
    Cursor& operator=(const Cursor&) = delete;
    Cursor& operator=(Cursor&& that) noexcept;
    ~Cursor();
    bool IsCursor() const override { return true; }
    HCURSOR Handle() const { return handle; }
    bool Standard() const { return standard; }
private:
    HCURSOR handle;
    bool standard;
};

Cursor LoadStandardCursor(StandardCursorId standardCursorId);
Cursor LoadCursorByName(const std::string& cursorName);
void SetCursor(const Cursor& cursor);

} // wing
