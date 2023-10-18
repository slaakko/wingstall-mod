// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing_package.make_setup;

import wing_package.package;
import wing_package.component;
import wingstall_config;
import dom;
import xpath;
import util;
import std.filesystem;

namespace wing_package {

void CoutLogger::WriteLine(const std::string& line)
{
    std::cout << line << std::endl;
}

std::string BoostIncludeDir()
{
    try
    {
        std::unique_ptr<xml::Document> configDoc = wingstall_config::ConfigurationDocument();
        if (configDoc)
        {
            xml::Element* rootElement = configDoc->DocumentElement();
            std::string boostIncludeDirAttr = rootElement->GetAttribute("boostIncludeDir");
            if (!boostIncludeDirAttr.empty())
            {
                return boostIncludeDirAttr;
            }
            throw std::runtime_error("'boostIncludeDir' attribute is empty or does not exist");
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << "could not access configuration document '" + wingstall_config::ConfigFilePath() + "': " + ex.what() << std::endl;
    }
    return wingstall_config::DefaultBoostIncludeDir();
}

std::string BoostLibDir()
{
    try
    {
        std::unique_ptr<xml::Document> configDoc = wingstall_config::ConfigurationDocument();
        if (configDoc)
        {
            xml::Element* rootElement = configDoc->DocumentElement();
            std::string boosLibDirAttr = rootElement->GetAttribute("boostLibDir");
            if (!boosLibDirAttr.empty())
            {
                return boosLibDirAttr;
            }
            throw std::runtime_error("'boostLibDir' attribute is empty or does not exist");
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << "could not access configuration document '" + wingstall_config::ConfigFilePath() + "': " + ex.what() << std::endl;
    }
    return wingstall_config::DefaultBoostLibDir();
}

std::string VCPlatformToolset()
{
    try
    {
        std::unique_ptr<xml::Document> configDoc = wingstall_config::ConfigurationDocument();
        if (configDoc)
        {
            xml::Element* rootElement = configDoc->DocumentElement();
            std::string vcPlatformToolsetAttr = rootElement->GetAttribute("vcPlatformToolset");
            if (!vcPlatformToolsetAttr.empty())
            {
                return vcPlatformToolsetAttr;
            }
            throw std::runtime_error("'vcPlatformToolset' attribute is empty or does not exist");
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << "could not access configuration document '" + wingstall_config::ConfigFilePath() + "': " + ex.what() << std::endl;
    }
    return wingstall_config::DefaultVCPlatformToolset();
}

std::string SetupIconResourceName()
{
    return "setup_icon";
}

std::map<std::string, std::string> MakeGuidMap();

void MakeMainFile(const std::string& packageBinFilePath, bool verbose, Logger* logger);

void MakeSolutionFile(const std::string& packageBinFilePath, std::map<std::string, std::string>& guidMap, bool verbose, Logger* logger);

void MakeDataFile(const std::string& packageBinFilePath, const std::string& appName, const std::string& appVersion, const std::string& compression, const std::string& defaultContainingDirPath,
    const std::string& installDirName, int64_t uncompressedPackageSize, bool verbose, Logger* logger);

void MakeProjectFile(const std::string& packageBinFilePath, bool verbose, Logger* logger, std::map<std::string, std::string>& guidMap);

void MakeSetup(const std::string& packageBinFilePath, bool verbose)
{
    CoutLogger coutLogger;
    MakeSetup(packageBinFilePath, verbose, &coutLogger);
}

void MakeSetup(const std::string& packageBinFilePath, bool verbose, Logger* logger)
{
    std::string packageInfoFilePath = util::Path::ChangeExtension(packageBinFilePath, ".info.xml");
    if (verbose && logger)
    {
        logger->WriteLine("> " + packageInfoFilePath);
    }
    std::unique_ptr<xml::Document> packageInfoDoc = xml::ReadDocument(packageInfoFilePath);
    std::unique_ptr<xpath::XPathObject> packageInfoObject = xpath::Evaluate("/packageInfo", packageInfoDoc.get());
    if (packageInfoObject)
    {
        if (packageInfoObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(packageInfoObject.get());
            int n = nodeSet->Length();
            if (n == 1)
            {
                xml::Node* node = (*nodeSet)[0];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    std::string appNameAttr = element->GetAttribute("appName");
                    if (appNameAttr.empty())
                    {
                        throw std::runtime_error("root element has no 'appName' attribute in package info document '" + packageInfoFilePath + "' line " +
                            std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
                    }
                    std::string appName = appNameAttr;
                    std::string appVersionAttr = element->GetAttribute("appVersion");
                    if (appVersionAttr.empty())
                    {
                        throw std::runtime_error("root element has no 'appVersion' attribute in package info document '" + packageInfoFilePath + "' line " +
                            std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
                    }
                    std::string appVersion = appVersionAttr;
                    std::string compressionAttr = element->GetAttribute("compression");
                    if (compressionAttr.empty())
                    {
                        throw std::runtime_error("root element has no 'compression' attribute in package info document '" + packageInfoFilePath + "' line " +
                            std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
                    }
                    std::string compression = compressionAttr;
                    std::string defaultContainingDirPathAttr = element->GetAttribute("defaultContainingDirPath");
                    if (defaultContainingDirPathAttr.empty())
                    {
                        throw std::runtime_error("root element has no 'defaultContainingDirPath' attribute in package info document '" + packageInfoFilePath + "' line " +
                            std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
                    }
                    std::string defaultContainingDirPath = defaultContainingDirPathAttr;
                    std::string installDirNameAttr = element->GetAttribute("installDirName");
                    if (installDirNameAttr.empty())
                    {
                        throw std::runtime_error("root element has no 'installDirName' attribute in package info document '" + packageInfoFilePath + "' line " +
                            std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
                    }
                    std::string installDirName = installDirNameAttr;
                    std::string uncompressedPackageSizeAttr = element->GetAttribute("uncompressedPackageSize");
                    if (uncompressedPackageSizeAttr.empty())
                    {
                        throw std::runtime_error("root element has no 'uncompressedPackageSize' attribute in package info document '" + packageInfoFilePath + "' line " +
                            std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
                    }
                    int64_t uncompressedPackageSize = std::stoll(uncompressedPackageSizeAttr);
                    std::map<std::string, std::string> guidMap = MakeGuidMap();
                    MakeSolutionFile(packageBinFilePath, guidMap, verbose, logger);
                    MakeDataFile(packageBinFilePath, appName, appVersion, compression, defaultContainingDirPath, installDirName, uncompressedPackageSize, verbose, logger);
                    MakeMainFile(packageBinFilePath, verbose, logger);
                    MakeProjectFile(packageBinFilePath, verbose, logger, guidMap);
                }
            }
            else
            {
                throw std::runtime_error("one 'packageInfo' element expected in package info document '" + packageInfoFilePath + "'");
            }
        }
    }
}

void MakeMainFile(const std::string& packageBinFilePath, bool verbose, Logger* logger)
{
    std::string mainFileBasePath = util::GetFullPath(
        util::Path::Combine(
            util::Path::Combine(
                util::Path::Combine(
                    util::Path::GetDirectoryName(
                        util::GetFullPath(packageBinFilePath)), 
                    "program"), 
                "setup"),
            "main"));
    std::string directoryPath = util::Path::GetDirectoryName(mainFileBasePath);
    std::error_code ec;
    std::filesystem::create_directories(directoryPath, ec);
    if (ec)
    {
        throw std::runtime_error("could not create directory '" + directoryPath + "': " + util::PlatformStringToUtf8(ec.message()));
    }
    std::string sourceFilePath = util::Path::ChangeExtension(mainFileBasePath, ".cpp");
    std::ofstream sourceFile(sourceFilePath);
    util::CodeFormatter sourceFormatter(sourceFile);
    sourceFormatter.WriteLine("#include <Windows.h>");
    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine("import data;");
    sourceFormatter.WriteLine("import wing_gui;");
    sourceFormatter.WriteLine("import wing_package;");
    sourceFormatter.WriteLine("import wing;");
    sourceFormatter.WriteLine("import util;");
    sourceFormatter.WriteLine("import std.core;");
    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine("void InitApplication(HINSTANCE instance)");
    sourceFormatter.WriteLine("{");
    sourceFormatter.IncIndent();
    sourceFormatter.WriteLine("util::Init();");
    sourceFormatter.WriteLine("wing::Init(instance);");
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("}");
    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine("int WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)");
    sourceFormatter.WriteLine("{");
    sourceFormatter.IncIndent();
    sourceFormatter.WriteLine("try");
    sourceFormatter.WriteLine("{");
    sourceFormatter.IncIndent();
    sourceFormatter.WriteLine("InitApplication(instance);");
    sourceFormatter.WriteLine("std::string currentExecutableName = util::Path::GetFileName(util::GetFullPath(util::GetPathToExecutable()));");
    sourceFormatter.WriteLine("wing::BinaryResourcePtr unicodeDBResource(currentExecutableName, setup::UnicodeDBResourceName());");
    sourceFormatter.WriteLine("util::CharacterTable::Instance().SetDeflateData(unicodeDBResource.Data(), unicodeDBResource.Size(), setup::UncompressedUnicodeDBSize());");
    sourceFormatter.WriteLine("wing::BinaryResourcePtr packageResource(currentExecutableName, setup::PackageResourceName());");
    sourceFormatter.WriteLine("wing_package::SetInfoItem(wing_package::InfoItemKind::appName, new wing_package::StringItem(setup::AppName()));");
    sourceFormatter.WriteLine("wing_package::SetInfoItem(wing_package::InfoItemKind::appVersion, new wing_package::StringItem(setup::AppVersion()));");
    sourceFormatter.WriteLine("wing_package::SetInfoItem(wing_package::InfoItemKind::installDirName, new wing_package::StringItem(setup::InstallDirName()));");
    sourceFormatter.WriteLine("wing_package::SetInfoItem(wing_package::InfoItemKind::defaultContainingDirPath, new wing_package::StringItem(setup::DefaultContainingDirPath()));");
    sourceFormatter.WriteLine("wing_package::SetInfoItem(wing_package::InfoItemKind::compression, new wing_package::IntegerItem(static_cast<int64_t>(setup::Compression())));");
    sourceFormatter.WriteLine("wing_package::SetInfoItem(wing_package::InfoItemKind::dataSource, new wing_package::IntegerItem(static_cast<int64_t>(wing_package::DataSource::memory)));");
    sourceFormatter.WriteLine("wing_package::SetInfoItem(wing_package::InfoItemKind::packageDataAddress, new wing_package::IntegerItem(reinterpret_cast<int64_t>(packageResource.Data())));");
    sourceFormatter.WriteLine("wing_package::SetInfoItem(wing_package::InfoItemKind::compressedPackageSize, new wing_package::IntegerItem(packageResource.Size()));");
    sourceFormatter.WriteLine("wing_package::SetInfoItem(wing_package::InfoItemKind::uncompressedPackageSize, new wing_package::IntegerItem(setup::UncompressedPackageSize()));");
    sourceFormatter.WriteLine("wing::Icon& setupIcon = wing::Application::GetResourceManager().GetIcon(setup::SetupIconResourceName());");
    sourceFormatter.WriteLine("wing_package::Package package;");
    sourceFormatter.WriteLine("wing_gui::InstallWindow installWindow;");
    sourceFormatter.WriteLine("installWindow.SetPackage(&package);");
    sourceFormatter.WriteLine("installWindow.SetIcon(setupIcon);");
    sourceFormatter.WriteLine("installWindow.SetSmallIcon(setupIcon);");
    sourceFormatter.WriteLine("wing::Application::Run(installWindow);");
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("}");
    sourceFormatter.WriteLine("catch (const std::exception& ex)");
    sourceFormatter.WriteLine("{");
    sourceFormatter.IncIndent();
    sourceFormatter.WriteLine("wing::ShowErrorMessageBox(nullptr, ex.what());");
    sourceFormatter.WriteLine("return 1;");
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("}");
    sourceFormatter.WriteLine("return 0;");
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("}");
    sourceFormatter.WriteLine();

    if (verbose && logger)
    {
        logger->WriteLine("==> " + sourceFilePath);
    }
}

std::map<std::string, std::string> MakeGuidMap()
{
    std::map<std::string, std::string> guidMap;
    guidMap["setup"] = "{" + util::ToString(util::random_uuid()) + "}";
    guidMap["util"] = "{C059187A-5946-409B-835E-6CBDC4B40493}";
    guidMap["wing_gui"] = "{97D97A57-6715-4100-BBD3-5587576BA8D0}";
    guidMap["wing"] = "{1303F2DE-F74E-4D40-BA58-3FE56AD912C2}";
    guidMap["wing_package"] = "{C33BC827-8238-4E4D-B2D7-8514891E74B1}";
    guidMap["wingstall_config"] = "{6E9AB84A-D526-431E-9DBB-550561FFD3FD}";
    guidMap["dom"] = "{E3F5A374-66FD-441F-B1D6-96908C0AF50B}";
    guidMap["rex"] = "{6C4BA617-7496-41BA-B7CE-7D17376B206B}";
    guidMap["lexer"] = "{73AFCD9A-9A1B-4E0E-A684-580BDAC40CCC}";
    guidMap["parser"] = "{4C27F999-9C62-4268-8DFB-496CBEE44634}";
    guidMap["xpath"] = "{0D607D43-EFE0-473D-A1C8-A5FE48971240}";
    guidMap["xml"] = "{5B6C01FC-A441-47F2-A830-29ADD514CC9A}";
    guidMap["util_inc"] = "{1A359821-A0AC-4B59-8958-ACE058417371}";
    guidMap["zlibstat"] = "{745DEC58-EBB3-47A9-A9B8-4C6627C01BF8}";
    return guidMap;
}

void MakeSolutionFile(const std::string& packageBinFilePath, std::map<std::string, std::string>& guidMap, bool verbose, Logger* logger)
{
    std::string solutionFilePath = util::GetFullPath(
        util::Path::Combine(
            util::Path::Combine(
                util::Path::GetDirectoryName(
                    util::GetFullPath(packageBinFilePath)),
                "program"),
            "setup.sln"));
    std::string directoryPath = util::Path::GetDirectoryName(solutionFilePath);
    std::error_code ec;
    std::filesystem::create_directories(directoryPath, ec);
    if (ec)
    {
        throw std::runtime_error("could not create directory '" + directoryPath + "': " + util::PlatformStringToUtf8(ec.message()));
    }
    std::string wingstallRootPath = util::GetFullPath(util::WingstallRoot());
    std::ofstream solutionFile(solutionFilePath);
    util::CodeFormatter formatter(solutionFile);
    formatter.WriteLine("Microsoft Visual Studio Solution File, Format Version 12.00");
    formatter.WriteLine("# Visual Studio Version 17");
    formatter.WriteLine("VisualStudioVersion = 17.7.34202.233");
    formatter.WriteLine("MinimumVisualStudioVersion = 10.0.40219.1");
    formatter.WriteLine("Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"setup\", \"setup\\setup.vcxproj\", \"" + guidMap["setup"] + "\"");
    formatter.WriteLine("EndProject");
    formatter.WriteLine("Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"util\", \"" + 
        util::MakeNativePath(util::GetFullPath(util::Path::Combine(wingstallRootPath, "util\\util.vcxproj"))) + "\", \"" + guidMap["util"] + "\"");
    formatter.WriteLine("    ProjectSection(ProjectDependencies) = postProject");
    formatter.WriteLine("        " + guidMap["zlibstat"] + " = " + guidMap["zlibstat"]);
    formatter.WriteLine("    EndProjectSection");
    formatter.WriteLine("EndProject");
    formatter.WriteLine("Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"wing_gui\", \"" + 
        util::MakeNativePath(util::GetFullPath(util::Path::Combine(wingstallRootPath, "wing_gui\\wing_gui.vcxproj"))) + "\", \"" + guidMap["wing_gui"] + "\"");
    formatter.WriteLine("EndProject");
    formatter.WriteLine("Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"wing\", \"" + 
        util::MakeNativePath(util::GetFullPath(util::Path::Combine(wingstallRootPath, "wing\\wing.vcxproj"))) + "\", \"" + guidMap["wing"] + "\"");
    formatter.WriteLine("EndProject");
    formatter.WriteLine("Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"wing_package\", \"" + 
        util::MakeNativePath(util::GetFullPath(util::Path::Combine(wingstallRootPath, "wing_package\\wing_package.vcxproj"))) + "\", \"" + guidMap["wing_package"] + "\"");
    formatter.WriteLine("EndProject");
    formatter.WriteLine("Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"wingstall_config\", \"" + 
        util::MakeNativePath(util::GetFullPath(util::Path::Combine(wingstallRootPath, "wingstall_config\\wingstall_config.vcxproj"))) + "\", \"" + guidMap["wingstall_config"] + "\"");
    formatter.WriteLine("EndProject");
    formatter.WriteLine("Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"dom\", \"" + 
        util::MakeNativePath(util::GetFullPath(util::Path::Combine(wingstallRootPath, "dom\\dom.vcxproj"))) + "\", \"" + guidMap["dom"] + "\"");
    formatter.WriteLine("EndProject");
    formatter.WriteLine("Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"rex\", \"" + 
        util::MakeNativePath(util::GetFullPath(util::Path::Combine(wingstallRootPath, "rex\\rex.vcxproj"))) + "\", \"" + guidMap["rex"] + "\"");
    formatter.WriteLine("EndProject");
    formatter.WriteLine("Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"lexer\", \"" + 
        util::MakeNativePath(util::GetFullPath(util::Path::Combine(wingstallRootPath, "lexer\\lexer.vcxproj"))) + "\", \"" + guidMap["lexer"] + "\"");
    formatter.WriteLine("EndProject");
    formatter.WriteLine("Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"parser\", \"" + 
        util::MakeNativePath(util::GetFullPath(util::Path::Combine(wingstallRootPath, "parser\\parser.vcxproj"))) + "\", \"" + guidMap["parser"] + "\"");
    formatter.WriteLine("EndProject");
    formatter.WriteLine("Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"xpath\", \"" + 
        util::MakeNativePath(util::GetFullPath(util::Path::Combine(wingstallRootPath, "xpath\\xpath.vcxproj"))) + "\", \"" + guidMap["xpath"] + "\"");
    formatter.WriteLine("EndProject");
    formatter.WriteLine("Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"xml\", \"" + 
        util::MakeNativePath(util::GetFullPath(util::Path::Combine(wingstallRootPath, "xml\\xml.vcxproj"))) + "\", \"" + guidMap["xml"] + "\"");
    formatter.WriteLine("EndProject");
    formatter.WriteLine("Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"util_inc\", \"" + 
        util::MakeNativePath(util::GetFullPath(util::Path::Combine(wingstallRootPath, "util_inc\\util_inc.vcxproj"))) + "\", \"" + guidMap["util_inc"] + "\"");
    formatter.WriteLine("EndProject");
    formatter.WriteLine("Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"zlibstat\", \"" + 
        util::MakeNativePath(util::GetFullPath(util::Path::Combine(wingstallRootPath, "ext\\zlib-1.2.11\\contrib\\vstudio\\vc14\\zlibstat.vcxproj"))) + "\", \"" +
        guidMap["zlibstat"] + "\"");
    formatter.WriteLine("EndProject");
    formatter.WriteLine("Global");
    formatter.WriteLine("    GlobalSection(SolutionConfigurationPlatforms) = preSolution");
    formatter.WriteLine("        Debug|x64 = Debug|x64");
    formatter.WriteLine("        Release|x64 = Release|x64");
    formatter.WriteLine("    EndGlobalSection");
    formatter.WriteLine("    GlobalSection(ProjectConfigurationPlatforms) = postSolution");
    formatter.WriteLine("        " + guidMap["setup"] + ".Debug|x64.ActiveCfg = Debug|x64");
    formatter.WriteLine("        " + guidMap["setup"] + ".Debug|x64.Build.0 = Debug|x64");
    formatter.WriteLine("        " + guidMap["setup"] + ".Release|x64.ActiveCfg = Release|x64");
    formatter.WriteLine("        " + guidMap["setup"] + ".Release|x64.Build.0 = Release|x64");
    formatter.WriteLine("        " + guidMap["util"] + ".Debug|x64.ActiveCfg = Debug|x64");
    formatter.WriteLine("        " + guidMap["util"] + ".Debug|x64.Build.0 = Debug|x64");
    formatter.WriteLine("        " + guidMap["util"] + ".Release|x64.ActiveCfg = Release|x64");
    formatter.WriteLine("        " + guidMap["util"] + ".Release|x64.Build.0 = Release|x64");
    formatter.WriteLine("        " + guidMap["wing_gui"] + ".Debug|x64.ActiveCfg = Debug|x64");
    formatter.WriteLine("        " + guidMap["wing_gui"] + ".Debug|x64.Build.0 = Debug|x64");
    formatter.WriteLine("        " + guidMap["wing_gui"] + ".Release|x64.ActiveCfg = Release|x64");
    formatter.WriteLine("        " + guidMap["wing_gui"] + ".Release|x64.Build.0 = Release|x64");
    formatter.WriteLine("        " + guidMap["wing"] + ".Debug|x64.ActiveCfg = Debug|x64");
    formatter.WriteLine("        " + guidMap["wing"] + ".Debug|x64.Build.0 = Debug|x64");
    formatter.WriteLine("        " + guidMap["wing"] + ".Release|x64.ActiveCfg = Release|x64");
    formatter.WriteLine("        " + guidMap["wing"] + ".Release|x64.Build.0 = Release|x64");
    formatter.WriteLine("        " + guidMap["wing_package"] + ".Debug|x64.ActiveCfg = Debug|x64");
    formatter.WriteLine("        " + guidMap["wing_package"] + ".Debug|x64.Build.0 = Debug|x64");
    formatter.WriteLine("        " + guidMap["wing_package"] + ".Release|x64.ActiveCfg = Release|x64");
    formatter.WriteLine("        " + guidMap["wing_package"] + ".Release|x64.Build.0 = Release|x64");
    formatter.WriteLine("        " + guidMap["wingstall_config"] + ".Debug|x64.ActiveCfg = Debug|x64");
    formatter.WriteLine("        " + guidMap["wingstall_config"] + ".Debug|x64.Build.0 = Debug|x64");
    formatter.WriteLine("        " + guidMap["wingstall_config"] + ".Release|x64.ActiveCfg = Release|x64");
    formatter.WriteLine("        " + guidMap["wingstall_config"] + ".Release|x64.Build.0 = Release|x64");
    formatter.WriteLine("        " + guidMap["dom"] + ".Debug|x64.ActiveCfg = Debug|x64");
    formatter.WriteLine("        " + guidMap["dom"] + ".Debug|x64.Build.0 = Debug|x64");
    formatter.WriteLine("        " + guidMap["dom"] + ".Release|x64.ActiveCfg = Release|x64");
    formatter.WriteLine("        " + guidMap["dom"] + ".Release|x64.Build.0 = Release|x64");
    formatter.WriteLine("        " + guidMap["rex"] + ".Debug|x64.ActiveCfg = Debug|x64");
    formatter.WriteLine("        " + guidMap["rex"] + ".Debug|x64.Build.0 = Debug|x64");
    formatter.WriteLine("        " + guidMap["rex"] + ".Release|x64.ActiveCfg = Release|x64");
    formatter.WriteLine("        " + guidMap["rex"] + ".Release|x64.Build.0 = Release|x64");
    formatter.WriteLine("        " + guidMap["lexer"] + ".Debug|x64.ActiveCfg = Debug|x64");
    formatter.WriteLine("        " + guidMap["lexer"] + ".Debug|x64.Build.0 = Debug|x64");
    formatter.WriteLine("        " + guidMap["lexer"] + ".Release|x64.ActiveCfg = Release|x64");
    formatter.WriteLine("        " + guidMap["lexer"] + ".Release|x64.Build.0 = Release|x64");
    formatter.WriteLine("        " + guidMap["parser"] + ".Debug|x64.ActiveCfg = Debug|x64");
    formatter.WriteLine("        " + guidMap["parser"] + ".Debug|x64.Build.0 = Debug|x64");
    formatter.WriteLine("        " + guidMap["parser"] + ".Release|x64.ActiveCfg = Release|x64");
    formatter.WriteLine("        " + guidMap["parser"] + ".Release|x64.Build.0 = Release|x64");
    formatter.WriteLine("        " + guidMap["xpath"] + ".Debug|x64.ActiveCfg = Debug|x64");
    formatter.WriteLine("        " + guidMap["xpath"] + ".Debug|x64.Build.0 = Debug|x64");
    formatter.WriteLine("        " + guidMap["xpath"] + ".Release|x64.ActiveCfg = Release|x64");
    formatter.WriteLine("        " + guidMap["xpath"] + ".Release|x64.Build.0 = Release|x64");
    formatter.WriteLine("        " + guidMap["xml"] + ".Debug|x64.ActiveCfg = Debug|x64");
    formatter.WriteLine("        " + guidMap["xml"] + ".Debug|x64.Build.0 = Debug|x64");
    formatter.WriteLine("        " + guidMap["xml"] + ".Release|x64.ActiveCfg = Release|x64");
    formatter.WriteLine("        " + guidMap["xml"] + ".Release|x64.Build.0 = Release|x64");
    formatter.WriteLine("        " + guidMap["util_inc"] + ".Debug|x64.ActiveCfg = Debug|x64");
    formatter.WriteLine("        " + guidMap["util_inc"] + ".Debug|x64.Build.0 = Debug|x64");
    formatter.WriteLine("        " + guidMap["util_inc"] + ".Release|x64.ActiveCfg = Release|x64");
    formatter.WriteLine("        " + guidMap["util_inc"] + ".Release|x64.Build.0 = Release|x64");
    formatter.WriteLine("        " + guidMap["zlibstat"] + ".Debug|x64.ActiveCfg = Debug|x64");
    formatter.WriteLine("        " + guidMap["zlibstat"] + ".Debug|x64.Build.0 = Debug|x64");
    formatter.WriteLine("        " + guidMap["zlibstat"] + ".Release|x64.ActiveCfg = Release|x64");
    formatter.WriteLine("        " + guidMap["zlibstat"] + ".Release|x64.Build.0 = Release|x64");
    formatter.WriteLine("    EndGlobalSection");
    formatter.WriteLine("    GlobalSection(SolutionProperties) = preSolution");
    formatter.WriteLine("        HideSolutionNode = FALSE");
    formatter.WriteLine("    EndGlobalSection");
    formatter.WriteLine("    GlobalSection(ExtensibilityGlobals) = postSolution");
    formatter.WriteLine("        SolutionGuid = {858FE728-3925-47E4-A961-35FA6EC108F5}");
    formatter.WriteLine("    EndGlobalSection");
    formatter.WriteLine("EndGlobal");

    if (verbose && logger)
    {
        logger->WriteLine("==> " + solutionFilePath);
    }
}

void MakeDataFile(const std::string& packageBinFilePath, const std::string& appName, const std::string& appVersion, const std::string& compression, const std::string& defaultContainingDirPath,
    const std::string& installDirName, int64_t uncompressedPackageSize, bool verbose, Logger* logger)
{
    std::string wingstallRootDir = util::WingstallRoot();
    std::string compressedUnicodeDBFilePath = util::CharacterTable::Instance().DeflateFilePath();
    std::string setupIconFilePath = util::GetFullPath(util::Path::Combine(util::Path::Combine(wingstallRootDir, "icon"), "setup.ico"));
    std::string packageResourceName = util::Path::GetFileNameWithoutExtension(packageBinFilePath);
    std::string unicodeDBResourceName = "wng_unicode_db";
    int64_t uncompressedUnicodeDBSize = util::CharacterTable::Instance().GetUncompressedFileSize();

    std::string dataFileBasePath = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::GetDirectoryName(
        util::GetFullPath(packageBinFilePath)), "program"), "setup"), "data"));
    std::string directoryPath = util::Path::GetDirectoryName(dataFileBasePath);
    std::error_code ec;
    std::filesystem::create_directories(directoryPath, ec);
    if (ec)
    {
        throw std::runtime_error("could not create directory '" + directoryPath + "': " + util::PlatformStringToUtf8(ec.message()));
    }
    std::string resourceFilePath = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::Path::Combine(
        util::Path::GetDirectoryName(util::GetFullPath(packageBinFilePath)), "program"), "setup"), "resource.rc"));
    std::ofstream resourceFile(resourceFilePath);
    util::CodeFormatter resourceFormatter(resourceFile);
    resourceFormatter.WriteLine(unicodeDBResourceName + " RCDATA \"" + util::StringStr(util::MakeNativePath(compressedUnicodeDBFilePath)) + "\"");
    resourceFormatter.WriteLine(SetupIconResourceName() + " ICON \"" + util::StringStr(util::MakeNativePath(setupIconFilePath)) + "\"");
    resourceFormatter.WriteLine(packageResourceName + " RCDATA \"" + util::StringStr(util::MakeNativePath(packageBinFilePath)) + "\"");

    std::string cppmFilePath = util::Path::ChangeExtension(dataFileBasePath, ".cppm");
    std::string cppmFileName = util::Path::GetFileName(cppmFilePath);
    std::ofstream cppmFile(cppmFilePath);
    util::CodeFormatter cppmFormatter(cppmFile);
    std::string sourceFilePath = util::Path::ChangeExtension(dataFileBasePath, ".cpp");
    std::ofstream sourceFile(sourceFilePath);
    util::CodeFormatter sourceFormatter(sourceFile);
    int64_t size = std::filesystem::file_size(packageBinFilePath);
    if (verbose && logger)
    {
        logger->WriteLine("> " + packageBinFilePath);
    }

    cppmFormatter.WriteLine("export module data;");
    cppmFormatter.WriteLine();
    cppmFormatter.WriteLine("import std.core;");
    cppmFormatter.WriteLine();
    cppmFormatter.WriteLine("export namespace setup {");
    cppmFormatter.WriteLine();
    cppmFormatter.WriteLine("std::string PackageResourceName();");
    cppmFormatter.WriteLine();
    cppmFormatter.WriteLine("int64_t UncompressedPackageSize();");
    cppmFormatter.WriteLine();
    cppmFormatter.WriteLine("std::string UnicodeDBResourceName();");
    cppmFormatter.WriteLine();
    cppmFormatter.WriteLine("int64_t UncompressedUnicodeDBSize();");
    cppmFormatter.WriteLine();
    cppmFormatter.WriteLine("std::string SetupIconResourceName();");
    cppmFormatter.WriteLine();
    cppmFormatter.WriteLine("std::string AppName();");
    cppmFormatter.WriteLine();
    cppmFormatter.WriteLine("std::string AppVersion();");
    cppmFormatter.WriteLine();
    cppmFormatter.WriteLine("int Compression();");
    cppmFormatter.WriteLine();
    cppmFormatter.WriteLine("std::string DefaultContainingDirPath();");
    cppmFormatter.WriteLine();
    cppmFormatter.WriteLine("std::string InstallDirName();");
    cppmFormatter.WriteLine();
    cppmFormatter.WriteLine("} // setup");

    sourceFormatter.WriteLine("module data;");
    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine("namespace setup {");
    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine("std::string PackageResourceName()");
    sourceFormatter.WriteLine("{");
    sourceFormatter.IncIndent();
    sourceFormatter.WriteLine("return \"" + packageResourceName + "\";");
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("}");
    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine("int64_t UncompressedPackageSize()");
    sourceFormatter.WriteLine("{");
    sourceFormatter.IncIndent();
    sourceFormatter.WriteLine("return " + std::to_string(uncompressedPackageSize) + ";");
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("}");
    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine("std::string UnicodeDBResourceName()");
    sourceFormatter.WriteLine("{");
    sourceFormatter.IncIndent();
    sourceFormatter.WriteLine("return \"" + unicodeDBResourceName + "\";");
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("}");
    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine("int64_t UncompressedUnicodeDBSize()");
    sourceFormatter.WriteLine("{");
    sourceFormatter.IncIndent();
    sourceFormatter.WriteLine("return " + std::to_string(uncompressedUnicodeDBSize) + ";");
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("}");
    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine("std::string SetupIconResourceName()");
    sourceFormatter.WriteLine("{");
    sourceFormatter.IncIndent();
    sourceFormatter.WriteLine("return \"" + SetupIconResourceName() + "\";");
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("}");
    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine("std::string AppName()");
    sourceFormatter.WriteLine("{");
    sourceFormatter.IncIndent();
    sourceFormatter.WriteLine("return \"" + appName + "\";");
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("}");
    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine("std::string AppVersion()");
    sourceFormatter.WriteLine("{");
    sourceFormatter.IncIndent();
    sourceFormatter.WriteLine("return \"" + appVersion + "\";");
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("}");
    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine("int Compression()");
    sourceFormatter.WriteLine("{");
    sourceFormatter.IncIndent();
    sourceFormatter.WriteLine("return " + std::to_string(static_cast<int>(wing_package::ParseCompressionStr(compression))) + "; // " + compression);
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("}");
    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine("std::string DefaultContainingDirPath()");
    sourceFormatter.WriteLine("{");
    sourceFormatter.IncIndent();
    sourceFormatter.WriteLine("return \"" + defaultContainingDirPath + "\";");
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("}");
    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine("std::string InstallDirName()");
    sourceFormatter.WriteLine("{");
    sourceFormatter.IncIndent();
    sourceFormatter.WriteLine("return \"" + installDirName + "\";");
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("}");
    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine("} // setup");
    sourceFormatter.WriteLine();

    if (verbose && logger)
    {
        logger->WriteLine("==> " + cppmFilePath);
        logger->WriteLine("==> " + sourceFilePath);
        logger->WriteLine("==> " + resourceFilePath);
    }
}

void MakeProjectFile(const std::string& packageBinFilePath, bool verbose, Logger* logger, std::map<std::string, std::string>& guidMap)
{
    std::string projectGuidStr = guidMap["setup"];

    std::string wingstallRootDir = util::WingstallRoot();
    //std::string wingstallIncludeDir = wingstallRootDir;
    //std::string wingstallLibDir = util::Path::Combine(wingstallRootDir, "lib");
    std::string wingstallConfigDir = util::Path::Combine(wingstallRootDir, "config");
    std::string projectFilePath = util::GetFullPath(
        util::Path::Combine(
            util::Path::Combine(
                util::Path::Combine(
                    util::Path::GetDirectoryName(
                        util::GetFullPath(packageBinFilePath)), 
                    "program"), 
                "setup"),
            "setup.vcxproj"));
    std::string directoryPath = util::Path::GetDirectoryName(projectFilePath);
    std::error_code ec;
    std::filesystem::create_directories(directoryPath, ec);
    if (ec)
    {
        throw std::runtime_error("could not create directory '" + directoryPath + "': " + util::PlatformStringToUtf8(ec.message()));
    }
    std::ofstream projectFile(projectFilePath);
    util::CodeFormatter formatter(projectFile);
    xml::Document projectDoc;
    projectDoc.SetXmlEncoding("utf-8");
    projectDoc.SetXmlVersion("1.0");
    xml::Element* projectElement = xml::MakeElement("Project");
    projectElement->SetAttribute("DefaultTargets", "Build");
    projectElement->SetAttribute("xmlns", "http://schemas.microsoft.com/developer/msbuild/2003");

    xml::Element* itemGroupElement = xml::MakeElement("ItemGroup");
    itemGroupElement->SetAttribute("Label", "ProjectConfigurations");

    xml::Element* debugX64ConfigElement = xml::MakeElement("ProjectConfiguration");
    debugX64ConfigElement->SetAttribute("Include", "Debug|x64");
    xml::Element* debugConfigElement = xml::MakeElement("Configuration");
    debugX64ConfigElement->AppendChild(debugConfigElement);
    xml::Text* debugConfigText = xml::MakeText("Debug");
    debugConfigElement->AppendChild(debugConfigText);
    xml::Element* debugPlatformElement = xml::MakeElement("Platform");
    debugX64ConfigElement->AppendChild(debugPlatformElement);
    xml::Text* debugPlatformText = xml::MakeText("x64");
    debugPlatformElement->AppendChild(debugPlatformText);

    xml::Element* releaseX64ConfigElement = xml::MakeElement("ProjectConfiguration");
    releaseX64ConfigElement->SetAttribute("Include", "Release|x64");
    xml::Element* releaseConfigElement = xml::MakeElement("Configuration");
    releaseX64ConfigElement->AppendChild(releaseConfigElement);
    xml::Text* releaseConfigText = xml::MakeText("Release");
    releaseConfigElement->AppendChild(releaseConfigText);
    xml::Element* releasePlatformElement = xml::MakeElement("Platform");
    releaseX64ConfigElement->AppendChild(releasePlatformElement);
    xml::Text* releasePlatformText = xml::MakeText("x64");
    releasePlatformElement->AppendChild(releasePlatformText);

    itemGroupElement->AppendChild(debugX64ConfigElement);
    itemGroupElement->AppendChild(releaseX64ConfigElement);
    projectElement->AppendChild(itemGroupElement);

    // Globals PropertyGroup
    xml::Element* globalsPropertyGroup = xml::MakeElement("PropertyGroup");
    globalsPropertyGroup->SetAttribute("Label", "Globals");
    projectElement->AppendChild(globalsPropertyGroup);
    xml::Element* vcProjectVersionElement = xml::MakeElement("VCProjectVersion");
    globalsPropertyGroup->AppendChild(vcProjectVersionElement);
    xml::Text* projectVersionText = xml::MakeText("17.0");
    vcProjectVersionElement->AppendChild(projectVersionText);
    xml::Element* keywordElement = xml::MakeElement("Keyword");
    globalsPropertyGroup->AppendChild(keywordElement);
    xml::Text* keywordText = xml::MakeText("Win32Proj");
    keywordElement->AppendChild(keywordText);

    xml::Element* projectGuidElement = xml::MakeElement("ProjectGuid");
    globalsPropertyGroup->AppendChild(projectGuidElement);
    xml::Text* projectGuidText = xml::MakeText(projectGuidStr);
    projectGuidElement->AppendChild(projectGuidText);

    xml::Element* rootNamespaceElement = xml::MakeElement("RootNamespace");
    globalsPropertyGroup->AppendChild(rootNamespaceElement);
    xml::Text* rootNamespaceText = xml::MakeText("setup");
    rootNamespaceElement->AppendChild(rootNamespaceText);

    xml::Element* windowsTargetPlatformVersionElement = xml::MakeElement("WindowsTargetPlatformVersion");
    globalsPropertyGroup->AppendChild(windowsTargetPlatformVersionElement);
    xml::Text* windowsTargetPlatformVersionText = xml::MakeText("10.0");
    windowsTargetPlatformVersionElement->AppendChild(windowsTargetPlatformVersionText);

    // Import Project
    xml::Element* importProjectElement = xml::MakeElement("Import");
    projectElement->AppendChild(importProjectElement);
    importProjectElement->SetAttribute("Project", "$(VCTargetsPath)\\Microsoft.Cpp.Default.props");

    // DebugX64Config PropertyGroup
    xml::Element* debugX64ConfigPropertyGroup = xml::MakeElement("PropertyGroup");
    projectElement->AppendChild(debugX64ConfigPropertyGroup);
    debugX64ConfigPropertyGroup->SetAttribute("Condition", "'$(Configuration)|$(Platform)'=='Debug|x64'");
    debugX64ConfigPropertyGroup->SetAttribute("Label", "Configuration");

    xml::Element* debugX64ConfigConfigurationTypeElement = xml::MakeElement("ConfigurationType");
    debugX64ConfigPropertyGroup->AppendChild(debugX64ConfigConfigurationTypeElement);
    xml::Text* debugX64ConfigConfigurationTypeText = xml::MakeText("Application");
    debugX64ConfigConfigurationTypeElement->AppendChild(debugX64ConfigConfigurationTypeText);

    xml::Element* debugX64ConfigUseDebugLibrariesElement = xml::MakeElement("UseDebugLibraries");
    debugX64ConfigPropertyGroup->AppendChild(debugX64ConfigUseDebugLibrariesElement);
    xml::Text* debugX64ConfigUseDebugLibrariesText = xml::MakeText("true");
    debugX64ConfigUseDebugLibrariesElement->AppendChild(debugX64ConfigUseDebugLibrariesText);

    xml::Element* debugX64ConfigPlatformToolsetElement = xml::MakeElement("PlatformToolset");
    debugX64ConfigPropertyGroup->AppendChild(debugX64ConfigPlatformToolsetElement);
    xml::Text* debugX64ConfigPlatformToolsetText = xml::MakeText(VCPlatformToolset());
    debugX64ConfigPlatformToolsetElement->AppendChild(debugX64ConfigPlatformToolsetText);

    xml::Element* debugX64ConfigCharacterSetElement = xml::MakeElement("CharacterSet");
    debugX64ConfigPropertyGroup->AppendChild(debugX64ConfigCharacterSetElement);
    xml::Text* debugX64ConfigCharacterSetText = xml::MakeText("Unicode");
    debugX64ConfigCharacterSetElement->AppendChild(debugX64ConfigCharacterSetText);

    // ReleaseX64Config PropertyGroup
    xml::Element* releaseX64ConfigPropertyGroup = xml::MakeElement("PropertyGroup");
    projectElement->AppendChild(releaseX64ConfigPropertyGroup);
    releaseX64ConfigPropertyGroup->SetAttribute("Condition", "'$(Configuration)|$(Platform)'=='Release|x64'");
    releaseX64ConfigPropertyGroup->SetAttribute("Label", "Configuration");

    xml::Element* releaseX64ConfigConfigurationTypeElement = xml::MakeElement("ConfigurationType");
    releaseX64ConfigPropertyGroup->AppendChild(releaseX64ConfigConfigurationTypeElement);
    xml::Text* releaseX64ConfigConfigurationTypeText = xml::MakeText("Application");
    releaseX64ConfigConfigurationTypeElement->AppendChild(releaseX64ConfigConfigurationTypeText);

    xml::Element* releaseX64ConfigUseDebugLibrariesElement = xml::MakeElement("UseDebugLibraries");
    releaseX64ConfigPropertyGroup->AppendChild(releaseX64ConfigUseDebugLibrariesElement);
    xml::Text* releaseX64ConfigUseDebugLibrariesText = xml::MakeText("false");
    releaseX64ConfigUseDebugLibrariesElement->AppendChild(releaseX64ConfigUseDebugLibrariesText);

    xml::Element* releaseX64ConfigPlatformToolsetElement = xml::MakeElement("PlatformToolset");
    releaseX64ConfigPropertyGroup->AppendChild(releaseX64ConfigPlatformToolsetElement);
    xml::Text* releaseX64ConfigPlatformToolsetText = xml::MakeText(VCPlatformToolset());
    releaseX64ConfigPlatformToolsetElement->AppendChild(releaseX64ConfigPlatformToolsetText);

    xml::Element* releaseX64ConfigWholeProgramOptimizationElement = xml::MakeElement("WholeProgramOptimization");
    releaseX64ConfigPropertyGroup->AppendChild(releaseX64ConfigWholeProgramOptimizationElement);
    xml::Text* releaseX64WholeProgramOptimizationText = xml::MakeText("true");
    releaseX64ConfigWholeProgramOptimizationElement->AppendChild(releaseX64WholeProgramOptimizationText);

    xml::Element* releaseX64ConfigCharacterSetElement = xml::MakeElement("CharacterSet");
    releaseX64ConfigPropertyGroup->AppendChild(releaseX64ConfigCharacterSetElement);
    xml::Text* releaseX64ConfigCharacterSetText = xml::MakeText("Unicode");
    releaseX64ConfigCharacterSetElement->AppendChild(releaseX64ConfigCharacterSetText);

    // Import Project2
    xml::Element* importProject2Element = xml::MakeElement("Import");
    projectElement->AppendChild(importProject2Element);
    importProject2Element->SetAttribute("Project", "$(VCTargetsPath)\\Microsoft.Cpp.props");

    // ImportExtensionSettings
    xml::Element* importExtensionSettingsElement = xml::MakeElement("ImportGroup");
    projectElement->AppendChild(importExtensionSettingsElement);
    importExtensionSettingsElement->SetAttribute("Label", "ExtensionSettings");

    // ImportShared
    xml::Element* importSharedElement = xml::MakeElement("ImportGroup");
    projectElement->AppendChild(importSharedElement);
    importSharedElement->SetAttribute("Label", "Shared");

    // Import x64/Debug PropertySheets
    xml::Element* importX64DebugPropertySheetsElement = xml::MakeElement("ImportGroup");
    projectElement->AppendChild(importX64DebugPropertySheetsElement);
    importX64DebugPropertySheetsElement->SetAttribute("Label", "PropertySheets");
    importX64DebugPropertySheetsElement->SetAttribute("Condition", "'$(Configuration)|$(Platform)'=='Debug|x64'");
    xml::Element* importX64DebugPropertySheetsProjectElement = xml::MakeElement("Import");
    importX64DebugPropertySheetsElement->AppendChild(importX64DebugPropertySheetsProjectElement);
    importX64DebugPropertySheetsProjectElement->SetAttribute("Project", "$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props");
    importX64DebugPropertySheetsProjectElement->SetAttribute("Condition", "exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')");
    importX64DebugPropertySheetsProjectElement->SetAttribute("Label", "LocalAppDataPlatform");
    xml::Element* importX64DebugBuildPropsElement = xml::MakeElement("Import");
    importX64DebugPropertySheetsElement->AppendChild(importX64DebugBuildPropsElement);
    std::string buildPropsPath = util::MakeNativePath(util::Path::Combine(wingstallConfigDir, "build.props"));
    importX64DebugBuildPropsElement->SetAttribute("Project", buildPropsPath);

    // Import x64/Release PropertySheets
    xml::Element* importX64ReleasePropertySheetsElement = xml::MakeElement("ImportGroup");
    projectElement->AppendChild(importX64ReleasePropertySheetsElement);
    importX64ReleasePropertySheetsElement->SetAttribute("Label", "PropertySheets");
    importX64ReleasePropertySheetsElement->SetAttribute("Condition", "'$(Configuration)|$(Platform)'=='Release|x64'");
    xml::Element* importX64ReleasePropertySheetsProjectElement = xml::MakeElement("Import");
    importX64ReleasePropertySheetsElement->AppendChild(importX64ReleasePropertySheetsProjectElement);
    importX64ReleasePropertySheetsProjectElement->SetAttribute("Project", "$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props");
    importX64ReleasePropertySheetsProjectElement->SetAttribute("Condition", "exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')");
    importX64ReleasePropertySheetsProjectElement->SetAttribute("Label", "LocalAppDataPlatform");
    xml::Element* importX64ReleaseBuildPropsElement = xml::MakeElement("Import");
    importX64ReleasePropertySheetsElement->AppendChild(importX64ReleaseBuildPropsElement);
    importX64ReleaseBuildPropsElement->SetAttribute("Project", buildPropsPath);

    // UserMacros
    xml::Element* userMacrosElement = xml::MakeElement("PropertyGroup");
    projectElement->AppendChild(userMacrosElement);
    userMacrosElement->SetAttribute("Label", "UserMacros");

    // TargetName DebugX64
    xml::Element* targetNameDebugX64ConditionElement = xml::MakeElement("PropertyGroup");
    projectElement->AppendChild(targetNameDebugX64ConditionElement);
    targetNameDebugX64ConditionElement->SetAttribute("Condition", "'$(Configuration)|$(Platform)'=='Debug|x64'");
    xml::Element* targetNameDebugX64Element = xml::MakeElement("TargetName");
    targetNameDebugX64ConditionElement->AppendChild(targetNameDebugX64Element);
    xml::Text* targetNameDebugX64Text = xml::MakeText("setupd");
    targetNameDebugX64Element->AppendChild(targetNameDebugX64Text);

    // TargetName ReleaseX64
    xml::Element* targetNameReleaseX64ConditionElement = xml::MakeElement("PropertyGroup");
    projectElement->AppendChild(targetNameReleaseX64ConditionElement);
    targetNameReleaseX64ConditionElement->SetAttribute("Condition", "'$(Configuration)|$(Platform)'=='Release|x64'");
    xml::Element* targetNameReleaseX64Element = xml::MakeElement("TargetName");
    targetNameReleaseX64ConditionElement->AppendChild(targetNameReleaseX64Element);
    xml::Text* targetNameReleaseX64Text = xml::MakeText("setup");
    targetNameReleaseX64Element->AppendChild(targetNameReleaseX64Text);

    // ItemDefinitionGroupClCompile DebugX64

    xml::Element* debugX64ItemDefinitionGroupCompileElement = xml::MakeElement("ItemDefinitionGroup");
    projectElement->AppendChild(debugX64ItemDefinitionGroupCompileElement);
    debugX64ItemDefinitionGroupCompileElement->SetAttribute("Condition", "'$(Configuration)|$(Platform)'=='Debug|x64'");

    xml::Element* debugX64CompileElement = xml::MakeElement("ClCompile");
    debugX64ItemDefinitionGroupCompileElement->AppendChild(debugX64CompileElement);

    xml::Element* debugX64WarningLevelElement = xml::MakeElement("WarningLevel");
    debugX64CompileElement->AppendChild(debugX64WarningLevelElement);
    xml::Text* debugX64WarningLevelText = xml::MakeText("Level3");
    debugX64WarningLevelElement->AppendChild(debugX64WarningLevelText);

    xml::Element* debugX64SDLCheckElement = xml::MakeElement("SDLCheck");
    debugX64CompileElement->AppendChild(debugX64SDLCheckElement);
    xml::Text* debugX64SDLCheckText = xml::MakeText("true");
    debugX64SDLCheckElement->AppendChild(debugX64SDLCheckText);

    xml::Element* debugX64PreprocessorElement = xml::MakeElement("PreprocessorDefinitions");
    debugX64CompileElement->AppendChild(debugX64PreprocessorElement);
    xml::Text* debugX64PreprocessorText = xml::MakeText("_DEBUG;_WINDOWS;%(PreprocessorDefinitions)");
    debugX64PreprocessorElement->AppendChild(debugX64PreprocessorText);

    xml::Element* debugX64ConformanceElement = xml::MakeElement("ConformanceMode");
    debugX64CompileElement->AppendChild(debugX64ConformanceElement);
    xml::Text* debugX64ConformanceText = xml::MakeText("true");
    debugX64ConformanceElement->AppendChild(debugX64ConformanceText);

    xml::Element* debugX64LanguageStandardElement = xml::MakeElement("LanguageStandard");
    debugX64CompileElement->AppendChild(debugX64LanguageStandardElement);
    xml::Text* debugX64LanguageStandardText = xml::MakeText("stdcpplatest");
    debugX64LanguageStandardElement->AppendChild(debugX64LanguageStandardText);

    xml::Element* debugX64MultiprocessCompilationElement = xml::MakeElement("MultiProcessorCompilation");
    debugX64CompileElement->AppendChild(debugX64MultiprocessCompilationElement);
    xml::Text* debugX64MultiprocessCompilationText = xml::MakeText("true");
    debugX64MultiprocessCompilationElement->AppendChild(debugX64MultiprocessCompilationText);

    xml::Element* debugX64EnableModulesElement = xml::MakeElement("EnableModules");
    debugX64CompileElement->AppendChild(debugX64EnableModulesElement);
    xml::Text* debugX64EnableModulesText = xml::MakeText("true");
    debugX64EnableModulesElement->AppendChild(debugX64EnableModulesText);

    xml::Element* debugX64BuildStlModulesElement = xml::MakeElement("BuildStlModules");
    debugX64CompileElement->AppendChild(debugX64BuildStlModulesElement);
    xml::Text* debugX64BuildStlModulesText = xml::MakeText("true");
    debugX64BuildStlModulesElement->AppendChild(debugX64BuildStlModulesText);

    xml::Element* debugX64IncludeElement = xml::MakeElement("AdditionalIncludeDirectories");
    debugX64CompileElement->AppendChild(debugX64IncludeElement);
    xml::Text* debugX64IncludeText = xml::MakeText(".;..;$(BOOST_INCLUDE_DIR)");
    debugX64IncludeElement->AppendChild(debugX64IncludeText);

    xml::Element* debugX64WarningsElement = xml::MakeElement("DisableSpecificWarnings");
    debugX64CompileElement->AppendChild(debugX64WarningsElement);
    xml::Text* debugX64WarningsText = xml::MakeText("5050;4267;4244;4146;4996");
    debugX64WarningsElement->AppendChild(debugX64WarningsText);

    xml::Element* debugX64LinkElement = xml::MakeElement("Link");
    debugX64ItemDefinitionGroupCompileElement->AppendChild(debugX64LinkElement);

    xml::Element* debugX64SubSystemElement = xml::MakeElement("SubSystem");
    debugX64LinkElement->AppendChild(debugX64SubSystemElement);
    xml::Text* debugX64SubSystemText = xml::MakeText("Windows");
    debugX64SubSystemElement->AppendChild(debugX64SubSystemText);

    xml::Element* debugX64DebugInfoElement = xml::MakeElement("GenerateDebugInformation");
    debugX64LinkElement->AppendChild(debugX64DebugInfoElement);
    xml::Text* debugX64DebugInfoText = xml::MakeText("true");
    debugX64DebugInfoElement->AppendChild(debugX64DebugInfoText);

    xml::Element* debugX64LibraryDirsElement = xml::MakeElement("AdditionalLibraryDirectories");
    debugX64LinkElement->AppendChild(debugX64LibraryDirsElement);
    xml::Text* debugX64LibraryDirsText = xml::MakeText("$(OutDir);$(BOOST_LIB_DIR)");
    debugX64LibraryDirsElement->AppendChild(debugX64LibraryDirsText);

    xml::Element* debugX64UACElement = xml::MakeElement("UACExecutionLevel");
    debugX64LinkElement->AppendChild(debugX64UACElement);
    xml::Text* debugX64UACText = xml::MakeText("RequireAdministrator");
    debugX64UACElement->AppendChild(debugX64UACText);

    // ItemDefinitionGroupClCompile ReleaseX64

    xml::Element* releaseX64ItemDefinitionGroupCompileElement = xml::MakeElement("ItemDefinitionGroup");
    projectElement->AppendChild(releaseX64ItemDefinitionGroupCompileElement);
    releaseX64ItemDefinitionGroupCompileElement->SetAttribute("Condition", "'$(Configuration)|$(Platform)'=='Release|x64'");

    xml::Element* releaseX64CompileElement = xml::MakeElement("ClCompile");
    releaseX64ItemDefinitionGroupCompileElement->AppendChild(releaseX64CompileElement);

    xml::Element* releaseX64WarningLevelElement = xml::MakeElement("WarningLevel");
    releaseX64CompileElement->AppendChild(releaseX64WarningLevelElement);
    xml::Text* releaseX64WarningLevelText = xml::MakeText("Level3");
    releaseX64WarningLevelElement->AppendChild(releaseX64WarningLevelText);

    xml::Element* releaseX64FunctionLevelLinkingElement = xml::MakeElement("FunctionLevelLinking");
    releaseX64CompileElement->AppendChild(releaseX64FunctionLevelLinkingElement);
    xml::Text* releaseX64FunctionLevelLinkingText = xml::MakeText("true");
    releaseX64FunctionLevelLinkingElement->AppendChild(releaseX64FunctionLevelLinkingText);

    xml::Element* releaseX64IntrinsicElement = xml::MakeElement("IntrinsicFunctions");
    releaseX64CompileElement->AppendChild(releaseX64IntrinsicElement);
    xml::Text* releaseX64IntrinsicsText = xml::MakeText("true");
    releaseX64IntrinsicElement->AppendChild(releaseX64IntrinsicsText);

    xml::Element* releaseX64SDLCheckElement = xml::MakeElement("SDLCheck");
    releaseX64CompileElement->AppendChild(releaseX64SDLCheckElement);
    xml::Text* releaseX64SDLCheckText = xml::MakeText("true");
    releaseX64SDLCheckElement->AppendChild(releaseX64SDLCheckText);

    xml::Element* releaseX64PreprocessorElement = xml::MakeElement("PreprocessorDefinitions");
    releaseX64CompileElement->AppendChild(releaseX64PreprocessorElement);
    xml::Text* releaseX64PreprocessorText = xml::MakeText("NDEBUG;_WINDOWS;%(PreprocessorDefinitions)");
    releaseX64PreprocessorElement->AppendChild(releaseX64PreprocessorText);

    xml::Element* releaseX64ConformanceElement = xml::MakeElement("ConformanceMode");
    releaseX64CompileElement->AppendChild(releaseX64ConformanceElement);
    xml::Text* releaseX64ConformanceText = xml::MakeText("true");
    releaseX64ConformanceElement->AppendChild(releaseX64ConformanceText);

    xml::Element* releaseX64LanguageStandardElement = xml::MakeElement("LanguageStandard");
    releaseX64CompileElement->AppendChild(releaseX64LanguageStandardElement);
    xml::Text* releaseX64LanguageStandardText = xml::MakeText("stdcpplatest");
    releaseX64LanguageStandardElement->AppendChild(releaseX64LanguageStandardText);

    xml::Element* releaseX64MultiprocessCompilationElement = xml::MakeElement("MultiProcessorCompilation");
    releaseX64CompileElement->AppendChild(releaseX64MultiprocessCompilationElement);
    xml::Text* releaseX64MultiprocessCompilationText = xml::MakeText("true");
    releaseX64MultiprocessCompilationElement->AppendChild(releaseX64MultiprocessCompilationText);

    xml::Element* releaseX64EnableModulesElement = xml::MakeElement("EnableModules");
    releaseX64CompileElement->AppendChild(releaseX64EnableModulesElement);
    xml::Text* releaseX64EnableModulesText = xml::MakeText("true");
    releaseX64EnableModulesElement->AppendChild(releaseX64EnableModulesText);

    xml::Element* releaseX64BuildStlModulesElement = xml::MakeElement("BuildStlModules");
    releaseX64CompileElement->AppendChild(releaseX64BuildStlModulesElement);
    xml::Text* releaseX64BuildStlModulesText = xml::MakeText("true");
    releaseX64BuildStlModulesElement->AppendChild(releaseX64BuildStlModulesText);

    xml::Element* releaseX64IncludeElement = xml::MakeElement("AdditionalIncludeDirectories");
    releaseX64CompileElement->AppendChild(releaseX64IncludeElement);
    xml::Text* releaseX64IncludeText = xml::MakeText(".;..;$(BOOST_INCLUDE_DIR)");
    releaseX64IncludeElement->AppendChild(releaseX64IncludeText);

    xml::Element* releaseX64WarningsElement = xml::MakeElement("DisableSpecificWarnings");
    releaseX64CompileElement->AppendChild(releaseX64WarningsElement);
    xml::Text* releaseX64WarningsText = xml::MakeText("5050;4267;4244;4146;4996");
    releaseX64WarningsElement->AppendChild(releaseX64WarningsText);

    xml::Element* releaseX64LinkElement = xml::MakeElement("Link");
    releaseX64ItemDefinitionGroupCompileElement->AppendChild(releaseX64LinkElement);

    xml::Element* releaseX64SubSystemElement = xml::MakeElement("SubSystem");
    releaseX64LinkElement->AppendChild(releaseX64SubSystemElement);
    xml::Text* releaseX64SubSystemText = xml::MakeText("Windows");
    releaseX64SubSystemElement->AppendChild(releaseX64SubSystemText);

    xml::Element* releaseX64EnableComDataFoldingElement = xml::MakeElement("EnableCOMDATFolding");
    releaseX64LinkElement->AppendChild(releaseX64EnableComDataFoldingElement);
    xml::Text* releaseX64EnableComDataFoldingText = xml::MakeText("true");
    releaseX64EnableComDataFoldingElement->AppendChild(releaseX64EnableComDataFoldingText);

    xml::Element* releaseX64OptimizeReferencesElement = xml::MakeElement("OptimizeReferences");
    releaseX64LinkElement->AppendChild(releaseX64OptimizeReferencesElement);
    xml::Text* releaseX64OptimizeReferencesText = xml::MakeText("true");
    releaseX64OptimizeReferencesElement->AppendChild(releaseX64OptimizeReferencesText);

    xml::Element* releaseX64DebugInfoElement = xml::MakeElement("GenerateDebugInformation");
    releaseX64LinkElement->AppendChild(releaseX64DebugInfoElement);
    xml::Text* releaseX64DebugInfoText = xml::MakeText("true");
    releaseX64DebugInfoElement->AppendChild(releaseX64DebugInfoText);

    xml::Element* releaseX64LibraryDirsElement = xml::MakeElement("AdditionalLibraryDirectories");
    releaseX64LinkElement->AppendChild(releaseX64LibraryDirsElement);
    xml::Text* releaseX64LibraryDirsText = xml::MakeText("$(OutDir);$(BOOST_LIB_DIR);");
    releaseX64LibraryDirsElement->AppendChild(releaseX64LibraryDirsText);

    xml::Element* releaseX64UACElement = xml::MakeElement("UACExecutionLevel");
    releaseX64LinkElement->AppendChild(releaseX64UACElement);
    xml::Text* releaseX64UACText = xml::MakeText("RequireAdministrator");
    releaseX64UACElement->AppendChild(releaseX64UACText);

    xml::Element* clCompileItemGroupElement = xml::MakeElement("ItemGroup");
    projectElement->AppendChild(clCompileItemGroupElement);

    xml::Element* clCompileDataEement = xml::MakeElement("ClCompile");
    clCompileItemGroupElement->AppendChild(clCompileDataEement);
    clCompileDataEement->SetAttribute("Include", "data.cpp");

    xml::Element* clCompileDataCppmEement = xml::MakeElement("ClCompile");
    clCompileItemGroupElement->AppendChild(clCompileDataCppmEement);
    clCompileDataCppmEement->SetAttribute("Include", "data.cppm");

    xml::Element* clCompileMainEement = xml::MakeElement("ClCompile");
    clCompileItemGroupElement->AppendChild(clCompileMainEement);
    clCompileMainEement->SetAttribute("Include", "main.cpp");

    xml::Element* projectReferencesItemGroupElement = xml::MakeElement("ItemGroup");
    projectElement->AppendChild(projectReferencesItemGroupElement);

    xml::Element* utilProjectReferenceElement = xml::MakeElement("ProjectReference");
    projectReferencesItemGroupElement->AppendChild(utilProjectReferenceElement);
    utilProjectReferenceElement->SetAttribute("Include", util::MakeNativePath(util::GetFullPath(util::Path::Combine(util::Path::Combine(wingstallRootDir, "util"), "util.vcxproj"))));
    xml::Element* utilProjectElement = xml::MakeElement("Project");
    xml::Text* utilProjectGuidText = xml::MakeText(guidMap["util"]);
    utilProjectElement->AppendChild(utilProjectGuidText);
    utilProjectReferenceElement->AppendChild(utilProjectElement);

    xml::Element* wingProjectReferenceElement = xml::MakeElement("ProjectReference");
    projectReferencesItemGroupElement->AppendChild(wingProjectReferenceElement);
    wingProjectReferenceElement->SetAttribute("Include", util::MakeNativePath(util::GetFullPath(util::Path::Combine(util::Path::Combine(wingstallRootDir, "wing"), "wing.vcxproj"))));
    xml::Element* wingProjectElement = xml::MakeElement("Project");
    xml::Text* wingProjectGuidText = xml::MakeText(guidMap["wing"]);
    wingProjectElement->AppendChild(wingProjectGuidText);
    wingProjectReferenceElement->AppendChild(wingProjectElement);

    xml::Element* wingGuiProjectReferenceElement = xml::MakeElement("ProjectReference");
    projectReferencesItemGroupElement->AppendChild(wingGuiProjectReferenceElement);
    wingGuiProjectReferenceElement->SetAttribute("Include", util::MakeNativePath(util::GetFullPath(util::Path::Combine(util::Path::Combine(wingstallRootDir, "wing_gui"), "wing_gui.vcxproj"))));
    xml::Element* wingGuiProjectElement = xml::MakeElement("Project");
    xml::Text* wingGuiProjectGuidText = xml::MakeText(guidMap["wing_gui"]);
    wingGuiProjectElement->AppendChild(wingGuiProjectGuidText);
    wingGuiProjectReferenceElement->AppendChild(wingGuiProjectElement);

    xml::Element* wingPackageProjectReferenceElement = xml::MakeElement("ProjectReference");
    projectReferencesItemGroupElement->AppendChild(wingPackageProjectReferenceElement);
    wingPackageProjectReferenceElement->SetAttribute("Include", util::MakeNativePath(util::GetFullPath(util::Path::Combine(util::Path::Combine(wingstallRootDir, "wing_package"), "wing_package.vcxproj"))));
    xml::Element* wingPackageProjectElement = xml::MakeElement("Project");
    xml::Text* wingPackageProjectGuidText = xml::MakeText(guidMap["wing_package"]);
    wingPackageProjectElement->AppendChild(wingPackageProjectGuidText);
    wingPackageProjectReferenceElement->AppendChild(wingPackageProjectElement);

    xml::Element* resourceItemGroupElement = xml::MakeElement("ItemGroup");
    projectElement->AppendChild(resourceItemGroupElement);

    xml::Element* resourceCompileResourceEement = xml::MakeElement("ResourceCompile");
    resourceItemGroupElement->AppendChild(resourceCompileResourceEement);
    resourceCompileResourceEement->SetAttribute("Include", "resource.rc");

    xml::Element* importTargetsElement = xml::MakeElement("Import");
    projectElement->AppendChild(importTargetsElement);
    importTargetsElement->SetAttribute("Project", "$(VCTargetsPath)\\Microsoft.Cpp.targets");

    xml::Element* importExtensionTargetsElement = xml::MakeElement("ImportGroup");
    projectElement->AppendChild(importExtensionTargetsElement);
    importExtensionTargetsElement->SetAttribute("Label", "ExtensionTargets");

    projectDoc.AppendChild(projectElement);
    projectDoc.Write(formatter);

    if (verbose && logger)
    {
        logger->WriteLine("==> " + projectFilePath);
    }
}

} // namespace wing_package
