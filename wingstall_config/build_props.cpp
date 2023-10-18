// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wingstall_config.build_props;

import wingstall_config.config;
import dom;
import util;

namespace wingstall_config {

void MakeBuildPropsFile(const std::string& boostIncludeDir, const std::string& boostLibDir)
{
    std::string configDir = util::GetFullPath(WingstallConfigDir());
    std::string buildPropsFilePath = util::Path::Combine(configDir, "build.props");
    xml::Document buildPropsDoc;
    buildPropsDoc.SetXmlVersion("1.0");
    buildPropsDoc.SetXmlEncoding("utf-8");
    std::unique_ptr<xml::Element> rootElement(xml::MakeElement("Project"));
    rootElement->SetAttribute("ToolsVersion", "4.0");
    rootElement->SetAttribute("xmlns", "http://schemas.microsoft.com/developer/msbuild/2003");
    std::unique_ptr<xml::Element> importGroupElement(xml::MakeElement("ImportGroup"));
    importGroupElement->SetAttribute("Label", "PropertySheets");
    rootElement->AppendChild(importGroupElement.release());

    std::unique_ptr<xml::Element> userMactorsPropertyGroupElement(xml::MakeElement("PropertyGroup"));
    userMactorsPropertyGroupElement->SetAttribute("Label", "UserMacros");
    std::unique_ptr<xml::Element> boostIncludeDirElement(xml::MakeElement("BOOST_INCLUDE_DIR"));
    std::unique_ptr<xml::Text> boostIncludeDirText(xml::MakeText(boostIncludeDir));
    boostIncludeDirElement->AppendChild(boostIncludeDirText.release());
    userMactorsPropertyGroupElement->AppendChild(boostIncludeDirElement.release());
    std::unique_ptr<xml::Element> boostLibDirElement(xml::MakeElement("BOOST_LIB_DIR"));
    std::unique_ptr<xml::Text> boostLibDirText(xml::MakeText(boostLibDir));
    boostLibDirElement->AppendChild(boostLibDirText.release());
    userMactorsPropertyGroupElement->AppendChild(boostLibDirElement.release());
    rootElement->AppendChild(userMactorsPropertyGroupElement.release());

    std::unique_ptr<xml::Element> buildPropertyGroupElement(xml::MakeElement("PropertyGroup"));
    std::unique_ptr<xml::Element> propertySheetDisplayNameElement(xml::MakeElement("_PropertySheetDisplayName"));
    std::unique_ptr<xml::Text> propertySheetDisplayNameText(xml::MakeText("Build"));
    propertySheetDisplayNameElement->AppendChild(propertySheetDisplayNameText.release());
    buildPropertyGroupElement->AppendChild(propertySheetDisplayNameElement.release());
    rootElement->AppendChild(buildPropertyGroupElement.release());

    std::unique_ptr<xml::Element> itemDefinitionGroupElement(xml::MakeElement("ItemDefinitionGroup"));
    std::unique_ptr<xml::Element> clCompileElement(xml::MakeElement("ClCompile"));
    std::unique_ptr<xml::Element> debugInformationFormatElement(xml::MakeElement("DebugInformationFormat"));
    std::unique_ptr<xml::Text> debugInformationFormatText(xml::MakeText("ProgramDatabase"));
    debugInformationFormatElement->AppendChild(debugInformationFormatText.release());
    clCompileElement->AppendChild(debugInformationFormatElement.release());
    itemDefinitionGroupElement->AppendChild(clCompileElement.release());
    rootElement->AppendChild(itemDefinitionGroupElement.release());

    std::unique_ptr<xml::Element> itemGroupElement(xml::MakeElement("ItemGroup"));

    std::unique_ptr<xml::Element> boostIncludeDirBuildMacroElement(xml::MakeElement("BuildMacro"));
    boostIncludeDirBuildMacroElement->SetAttribute("Include", "BOOST_INCLUDE_DIR");
    std::unique_ptr<xml::Element> boostIncludeDirValueElement(xml::MakeElement("Value"));
    std::unique_ptr<xml::Text> boostIncludeDirValueText(xml::MakeText("$(BOOST_INCLUDE_DIR)"));
    boostIncludeDirValueElement->AppendChild(boostIncludeDirValueText.release());
    boostIncludeDirBuildMacroElement->AppendChild(boostIncludeDirValueElement.release());
    itemGroupElement->AppendChild(boostIncludeDirBuildMacroElement.release());

    std::unique_ptr<xml::Element> boostLibDirBuildMacroElement(xml::MakeElement("BuildMacro"));
    boostLibDirBuildMacroElement->SetAttribute("Include", "BOOST_LIB_DIR");
    std::unique_ptr<xml::Element> boostLibDirValueElement(xml::MakeElement("Value"));
    std::unique_ptr<xml::Text> boostLibDirValueText(xml::MakeText("$(BOOST_LIB_DIR)"));
    boostLibDirValueElement->AppendChild(boostLibDirValueText.release());
    boostLibDirBuildMacroElement->AppendChild(boostLibDirValueElement.release());
    itemGroupElement->AppendChild(boostLibDirBuildMacroElement.release());

    rootElement->AppendChild(itemGroupElement.release());

    buildPropsDoc.AppendChild(rootElement.release());

    std::ofstream buildPropsNewFile(buildPropsFilePath + ".new");
    util::CodeFormatter newFormatter(buildPropsNewFile);
    newFormatter.SetIndentSize(2);
    buildPropsDoc.Write(newFormatter);

    std::ofstream buildPropsFile(buildPropsFilePath);
    util::CodeFormatter formatter(buildPropsFile);
    formatter.SetIndentSize(2);
    buildPropsDoc.Write(formatter);
}

} // namespace wingstall_config
