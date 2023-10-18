// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module dom.parser;

import dom.attribute_node;
import dom.element;
import dom.character_data;
import dom.comment;
import dom.text;
import dom.processing_instruction;
import dom.cdata_section;
import util;

namespace xml {

class DomDocumentHandler : public XmlContentHandler
{
public:
    DomDocumentHandler();
    std::unique_ptr<Document> GetDocument();
    void StartDocument() override;
    void EndDocument() override;
    void Version(const std::u32string& xmlVersion) override;
    void Standalone(bool standalone) override;
    void Encoding(const std::u32string& encoding) override;
    void Text(const std::u32string& text) override;
    void Comment(const std::u32string& comment) override;
    void PI(const std::u32string& target, const std::u32string& data) override;
    void CDataSection(const std::u32string& data) override;
    void StartElement(const std::u32string& namespaceUri, const std::u32string& localName, const std::u32string& qualifiedName, const Attributes& attributes, 
        const lexer::SourcePos& sourcePos) override;
    void EndElement(const std::u32string& namespaceUri, const std::u32string& localName, const std::u32string& qualifiedName) override;
    void SkippedEntity(const std::u32string& entityName) override;
private:
    std::unique_ptr<Document> document;
    std::unique_ptr<Element> currentElement;
    std::stack<std::unique_ptr<Element>> elementStack;
    std::u32string textContent;
    void AddTextContent();
    void AddTextContent(bool addSpace);
};

DomDocumentHandler::DomDocumentHandler() : document(new Document())
{
}

std::unique_ptr<Document> DomDocumentHandler::GetDocument()
{
    return std::move(document);
}

void DomDocumentHandler::AddTextContent()
{
    AddTextContent(false);
}

void DomDocumentHandler::AddTextContent(bool addSpace)
{
    if (currentElement)
    {
        std::u32string text = util::TrimAll(textContent);
        if (!text.empty())
        {
            if (addSpace)
            {
                text.append(1, ' ');
            }
            std::unique_ptr<xml::Text> textNode(xml::MakeText(util::ToUtf8(text)));
            currentElement->AppendChild(textNode.release());
        }
    }
    textContent.clear();
}

void DomDocumentHandler::StartDocument()
{
    // todo
}

void DomDocumentHandler::EndDocument()
{
    // todo
}

void DomDocumentHandler::Version(const std::u32string& xmlVersion)
{
    document->SetXmlVersion(util::ToUtf8(xmlVersion));
}

void DomDocumentHandler::Standalone(bool standalone)
{
    document->SetXmlStandalone(standalone);
}

void DomDocumentHandler::Encoding(const std::u32string& encoding)
{
    document->SetXmlEncoding(util::ToUtf8(encoding));
}

void DomDocumentHandler::Text(const std::u32string& text)
{
    textContent.append(text);
}

void DomDocumentHandler::Comment(const std::u32string& comment)
{
    AddTextContent();
    std::unique_ptr<xml::Comment> commentNode(xml::MakeComment(util::ToUtf8(comment)));
    if (currentElement)
    {
        currentElement->AppendChild(commentNode.release());
    }
    else
    {
        document->AppendChild(commentNode.release());
    }
}

void DomDocumentHandler::PI(const std::u32string& target, const std::u32string& data)
{
    AddTextContent();
    std::unique_ptr<xml::ProcessingInstruction> processingInstructionNode(xml::MakeProcessingInstruction(util::ToUtf8(target), util::ToUtf8(data)));
    if (currentElement)
    {
        currentElement->AppendChild(processingInstructionNode.release());
    }
    else
    {
        document->AppendChild(processingInstructionNode.release());
    }
}

void DomDocumentHandler::CDataSection(const std::u32string& data)
{
    AddTextContent();
    std::unique_ptr<xml::CDataSection> cdataSection(xml::MakeCDataSection(util::ToUtf8(data)));
    if (currentElement)
    {
        currentElement->AppendChild(cdataSection.release());
    }
    else
    {
        document->AppendChild(cdataSection.release());
    }
}

void DomDocumentHandler::StartElement(const std::u32string& namespaceUri, const std::u32string& localName, const std::u32string& qualifiedName, const Attributes& attributes, 
    const lexer::SourcePos& sourcePos)
{
    AddTextContent(true);
    elementStack.push(std::move(currentElement));
    std::map<std::u32string, std::unique_ptr<AttributeNode>> attrs;
    for (const Attribute& attr : attributes)
    {
        attrs[attr.QualifiedName()] = std::unique_ptr<AttributeNode>(MakeAttribute(util::ToUtf8(attr.QualifiedName()), util::ToUtf8(attr.Value())));
    }
    currentElement.reset(new xml::Element(sourcePos, util::ToUtf8(qualifiedName)));
    for (auto& p : attrs)
    {
        currentElement->AddAttribute(p.second.release());
    }
    currentElement->SetOwnerDocument(document.get());
    if (!namespaceUri.empty())
    {
        currentElement->SetNamespaceUri(util::ToUtf8(namespaceUri));
    }
}

void DomDocumentHandler::EndElement(const std::u32string& namespaceUri, const std::u32string& localName, const std::u32string& qualifiedName)
{
    AddTextContent();
    std::unique_ptr<Element> parentElement = std::move(elementStack.top());
    elementStack.pop();
    if (parentElement)
    {
        parentElement->AppendChild(currentElement.release());
        currentElement = std::move(parentElement);
    }
    else
    {
        document->AppendChild(currentElement.release());
    }
}

void DomDocumentHandler::SkippedEntity(const std::u32string& entityName)
{
    // todo
}

std::unique_ptr<Document> ParseDocument(const std::u32string& content, const std::string& systemId)
{
    return ParseDocument(content, systemId, Flags::none);
}

std::unique_ptr<Document> ParseDocument(const std::u32string& content, const std::string& systemId, Flags flags)
{
    DomDocumentHandler domDocumentHandler;
    ParseXmlContent(content, systemId, &domDocumentHandler, flags);
    return domDocumentHandler.GetDocument();
}

std::unique_ptr<Document> ReadDocument(const std::string& fileName)
{
    return ReadDocument(fileName, Flags::none);
}

std::unique_ptr<Document> ReadDocument(const std::string& fileName, Flags flags)
{
    std::u32string content = util::ToUtf32(util::ReadFile(fileName));
    return ParseDocument(content, fileName, flags);
}

void SendDocument(util::TcpSocket& socket, Document& document)
{
    std::stringstream sstream;
    util::CodeFormatter formatter(sstream);
    document.Write(formatter);
    Write(socket, sstream.str());
}

std::unique_ptr<Document> ReceiveDocument(util::TcpSocket& socket)
{
    std::string str = ReadStr(socket);
    if (str.empty()) return std::unique_ptr<Document>();
    std::u32string content = util::ToUtf32(str);
    std::unique_ptr<Document> doc = ParseDocument(content, "socket");
    return doc;
}

} // namespace dom
