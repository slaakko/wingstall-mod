// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.file_util;

import std.core;

export namespace wing {

void MoveFile(const std::string& from, const std::string& to);
void MoveFile(const std::string& from, const std::string& to, bool allowCopy, bool replaceExisting);
void RemoveOnReboot(const std::string& path);

} // wing
