// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

// ==========================================================
// Interface to a non-validating XML parser with SAX-like API
// Author: S. Laakko
// ==========================================================

export module xml.xml_parser_interface;

import xml.xml_content_handler;
import std.core;

export namespace xml {

enum class Flags : int
{
    none = 0, debug = 1 << 0
};

inline Flags operator&(Flags flags, Flags flag)
{
    return static_cast<Flags>(static_cast<int>(flags) & static_cast<int>(flag));
}

inline Flags operator|(Flags flags, Flags flag)
{
    return static_cast<Flags>(static_cast<int>(flags) | static_cast<int>(flag));
}

inline Flags operator~(Flags flags)
{
    return static_cast<Flags>(~static_cast<int>(flags));
}

//  ==================================================================================
//  ParseXmlFile parses given UTF-8 encoded XML file using given content handler.
//  ==================================================================================

void ParseXmlFile(const std::string& xmlFileName, XmlContentHandler* contentHandler);
void ParseXmlFile(const std::string& xmlFileName, XmlContentHandler* contentHandler, Flags flags);

//  ==================================================================================
//  ParseXmlContent parses given UTF-8 encoded XML string using given content handler.
//  systemId is used for error messages only. It can be for example a file name or URL 
//  that identifies the XML string to parse.
//  ===================================================================================

void ParseXmlContent(const std::string& xmlContent, const std::string& systemId, XmlContentHandler* contentHandler);
void ParseXmlContent(const std::string& xmlContent, const std::string& systemId, XmlContentHandler* contentHandler, Flags flags);

//  ===================================================================================
//  ParseXmlContent parses given UTF-32 encoded XML string using given content handler.
//  systemId is used for error messages only. It can be for example a file name or URL 
//  that identifies the XML string to parse.
//  ===================================================================================

void ParseXmlContent(const std::u32string& xmlContent, const std::string& systemId, XmlContentHandler* contentHandler);
void ParseXmlContent(const std::u32string& xmlContent, const std::string& systemId, XmlContentHandler* contentHandler, Flags flags);

} // namespace xml
