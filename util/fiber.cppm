// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.fiber;

import std.core;

export namespace util {

void* ConvertThreadToFiber(void* param);
void* CreateFiber(uint64_t stackSize, void* startAddress, void* param);
void SwitchToFiber(void* fiber);
void* GetFiberData();
void DeleteFiber(void* fiber);

} // namespace util
