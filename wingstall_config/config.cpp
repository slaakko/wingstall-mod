// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wingstall_config.config;

import util;
import std.filesystem;

namespace wingstall_config {

std::string WingstallConfigDir()
{
    std::string configDir = util::Path::Combine(util::WingstallRoot(), "config");
    std::filesystem::create_directories(configDir);
    return configDir;
}

std::string defaultBoostIncludeDir = "C:\\boost\\include\\boost-1_80";
std::string defaultBoostLibDir = "C:\\boost\\lib";
std::string defaultVCVarsFilePath = R"(C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat)";
std::string defaultVCPlatformToolset = "v143";

std::string ConfigFilePath()
{
    return util::Path::Combine(util::GetFullPath(WingstallConfigDir()), "configuration.xml");
}

std::string DefaultBoostIncludeDir()
{
    return defaultBoostIncludeDir;
}

std::string DefaultBoostLibDir()
{
    return defaultBoostLibDir;
}

std::string DefaultVCVarsFilePath()
{
    return defaultVCVarsFilePath;
}

std::string DefaultVCPlatformToolset()
{
    return defaultVCPlatformToolset;
}

std::unique_ptr<xml::Document> ConfigurationDocument()
{
    std::string operation;
    try
    {
        std::unique_ptr<xml::Document> configDoc;
        if (std::filesystem::exists(ConfigFilePath()))
        {
            operation = "read/write";
            configDoc = xml::ReadDocument(ConfigFilePath());
            std::string vcVarsFilePath = configDoc->DocumentElement()->GetAttribute("vcVarsFilePath");
            if (vcVarsFilePath.empty())
            {
                configDoc->DocumentElement()->SetAttribute("vcVarsFilePath", defaultVCVarsFilePath);
            }
            std::string vcPlatformToolset = configDoc->DocumentElement()->GetAttribute("vcPlatformToolset");
            if (vcPlatformToolset.empty())
            {
                configDoc->DocumentElement()->SetAttribute("vcPlatformToolset", defaultVCPlatformToolset);
            }
            std::ofstream configFile(ConfigFilePath());
            util::CodeFormatter formatter(configFile);
            configDoc->Write(formatter);
        }
        else
        {
            operation = "write";
            configDoc.reset(new xml::Document());
            xml::Element* rootElement = xml::MakeElement("configuration");
            configDoc->AppendChild(rootElement);
            rootElement->SetAttribute("boostIncludeDir", defaultBoostIncludeDir);
            rootElement->SetAttribute("boostLibDir", defaultBoostLibDir);
            rootElement->SetAttribute("vcVarsFilePath", defaultVCVarsFilePath);
            rootElement->SetAttribute("vcPlatformToolset", defaultVCPlatformToolset);
            std::ofstream configFile(ConfigFilePath());
            util::CodeFormatter formatter(configFile);
            configDoc->Write(formatter);
        }
        return configDoc;
    }
    catch (const std::exception& ex)
    {
        std::cerr << "could not " << operation << " configuration document '" + ConfigFilePath() + "': " + ex.what() << std::endl;;
    }
    return std::unique_ptr<xml::Document>();
}

std::string BoostIncludeDir(xml::Document* configDocument)
{
    return configDocument->DocumentElement()->GetAttribute("boostIncludeDir");
}

std::string BoostLibDir(xml::Document* configDocument)
{
    return configDocument->DocumentElement()->GetAttribute("boostLibDir");
}

std::string VCVarsFilePath(xml::Document* configDocument)
{
    return configDocument->DocumentElement()->GetAttribute("vcVarsFilePath");
}

std::string VCPlatformToolset(xml::Document* configDocument)
{
    return configDocument->DocumentElement()->GetAttribute("vcPlatformToolset");
}

void SetBoostIncludeDir(xml::Document* configDocument, const std::string& boostIncludeDir)
{
    configDocument->DocumentElement()->SetAttribute("boostIncludeDir", boostIncludeDir);
}

void SetBoostLibDir(xml::Document* configDocument, const std::string& boostLibDir)
{
    configDocument->DocumentElement()->SetAttribute("boostLibDir", boostLibDir);
}

void SetVCVarsFilePath(xml::Document* configDocument, const std::string& vcVarsFilePath)
{
    configDocument->DocumentElement()->SetAttribute("vcVarsFilePath", vcVarsFilePath);
}

void SetVCPlatformToolset(xml::Document* configDocument, const std::string& vcPlatformToolset)
{
    configDocument->DocumentElement()->SetAttribute("vcPlatformToolset", vcPlatformToolset);
}

void SaveConfiguration(xml::Document* configDocument)
{
    std::ofstream configFile(ConfigFilePath());
    util::CodeFormatter formatter(configFile);
    configDocument->Write(formatter);
}

} // namespace wingstall_config
