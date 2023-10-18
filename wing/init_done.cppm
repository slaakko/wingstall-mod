// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

export module wing.init_done;

import std.core;

export namespace wing {

void Init(HINSTANCE instance);
void Done();

} // wing
