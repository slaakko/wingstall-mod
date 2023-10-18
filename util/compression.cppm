// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.compression;

import std.core;

export namespace util {

enum class CompressionMode : int
{
    compress = 0, decompress = 1
};

} // namespace util
