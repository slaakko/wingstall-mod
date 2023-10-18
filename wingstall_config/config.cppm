// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wingstall_config.config;

import dom;
import std.core;

export namespace wingstall_config {

std::string WingstallConfigDir();
std::string ConfigFilePath();
std::string DefaultBoostIncludeDir();
std::string DefaultBoostLibDir();
std::string DefaultVCVarsFilePath();
std::string DefaultVCPlatformToolset();
std::unique_ptr<xml::Document> ConfigurationDocument();
std::string BoostIncludeDir(xml::Document* configDocument);
std::string BoostLibDir(xml::Document* configDocument);
std::string VCVarsFilePath(xml::Document* configDocument);
std::string VCPlatformToolset(xml::Document* configDocument);
void SetBoostIncludeDir(xml::Document* configDocument, const std::string& boostIncludeDir);
void SetBoostLibDir(xml::Document* configDocument, const std::string& boostLibDir);
void SetVCVarsFilePath(xml::Document* configDocument, const std::string& vcVarsFilePath);
void SetVCPlatformToolset(xml::Document* configDocument, const std::string& vcPlatformToolset);
void SaveConfiguration(xml::Document* configDocument);

} // namespace wingstall_config
