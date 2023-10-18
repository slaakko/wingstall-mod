// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module xml.xml_parser_interface;

import xml.xml_parser;
import xml.xml_processor;
import xml.rules;
import lexer;
import util;

namespace xml {

void ParseXmlFile(const std::string& xmlFileName, XmlContentHandler* contentHandler)
{
    ParseXmlFile(xmlFileName, contentHandler, Flags::none);
}

void ParseXmlFile(const std::string& xmlFileName, XmlContentHandler* contentHandler, Flags flags)
{
    std::string xmlContent = util::ReadFile(xmlFileName);
    ParseXmlContent(xmlContent, xmlFileName, contentHandler, flags);
}

void ParseXmlContent(const std::string& xmlContent, const std::string& systemId, XmlContentHandler* contentHandler)
{
    ParseXmlContent(util::ToUtf32(xmlContent), systemId, contentHandler, Flags::none);
}

void ParseXmlContent(const std::string& xmlContent, const std::string& systemId, XmlContentHandler* contentHandler, Flags flags)
{
    ParseXmlContent(util::ToUtf32(xmlContent), systemId, contentHandler, flags);
}

void ParseXmlContent(const std::u32string& xmlContent, const std::string& systemId, XmlContentHandler* contentHandler)
{
    ParseXmlContent(xmlContent, systemId, contentHandler, Flags::none);
}

void ParseXmlContent(const std::u32string& xmlContent, const std::string& systemId, XmlContentHandler* contentHandler, Flags flags)
{
    lexer::XmlParsingLog debugLog(std::cerr);
    TrivialLexer xmlLexer(xmlContent, systemId, 0);
    xmlLexer.SetFlag(lexer::LexerFlags::farthestError);
    xmlLexer.SetRuleNameVecPtr(GetRuleNameVecPtr());
    if ((flags & Flags::debug) != Flags::none)
    {
        xmlLexer.SetLog(&debugLog);
    }
    XmlProcessor xmlProcessor(xmlLexer, contentHandler);
    XmlParser::Parse(xmlLexer, &xmlProcessor);
}

} // namespace xml
