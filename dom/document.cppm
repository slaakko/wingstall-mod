// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module dom.document;

import std.core;
import dom.parent_node;
import dom.element;

export namespace xml {

class Document : public ParentNode
{
public:
    Document();
    Document(const lexer::SourcePos& sourcePos_);
    Element* DocumentElement() const { return documentElement; }
    bool XmlStandalone() const { return xmlStandalone; }
    void SetXmlStandalone(bool xmlStandalone_) { xmlStandalone = xmlStandalone_; }
    const std::string& XmlVersion() const { return xmlVersion; }
    void SetXmlVersion(const std::string& xmlVersion_) { xmlVersion = xmlVersion_; }
    const std::string& XmlEncoding() const { return xmlEncoding; }
    void SetXmlEncoding(const std::string& xmlEncoding_) { xmlEncoding = xmlEncoding_; }
    std::map<std::string, Element*>& Index() { return index; }
    Element* GetElementById(const std::string& elementId);
    void AppendChild(Node* child) override;
    void InsertBefore(Node* newChild, Node* refChild) override;
    std::unique_ptr<Node> RemoveChild(Node* child) override;
    std::unique_ptr<Node> ReplaceChild(Node* newChild, Node* oldChild) override;
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
private:
    friend class ParentNode;
    void InvalidateIndex();
    void CheckValidInsert(Node* node);
    Element* documentElement;
    bool indexValid;
    bool xmlStandalone;
    std::string xmlVersion;
    std::string xmlEncoding;
    std::map<std::string, Element*> index;
};

std::unique_ptr<Document> MakeDocument();

} // namespace xml
