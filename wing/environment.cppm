// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.environment;

import wing.registry;
import std.core;

export namespace wing {

bool HasSystemEnvironmentVariable(const std::string& name);

std::string GetSystemEnvironmentVariable(const std::string& name);

void SetSystemEnvironmentVariable(const std::string& name, const std::string& value, RegistryValueKind valueKind);

void DeleteSystemEnvironmentVariable(const std::string& name);

bool HasPathDirectory(const std::string& directory);

void AppendPathDirectory(const std::string& directory);

void RemovePathDirectory(const std::string& directory);

void BroadcastEnvironmentChangedMessage();

} // wing
