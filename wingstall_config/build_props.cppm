// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wingstall_config.build_props;

import std.core;

export namespace wingstall_config {

void MakeBuildPropsFile(const std::string& boostIncludeDir, const std::string& boostLibDir);

} // namespace wingstall_config
