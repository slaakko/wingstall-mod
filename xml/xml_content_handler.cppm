// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module xml.xml_content_handler;

import lexer.source_pos;
import std.core;

export namespace xml {

class Attribute
{
public:
    Attribute(const std::u32string& namespaceUri_, const std::u32string& localName_, const std::u32string& qualifiedName_, const std::u32string& value_);
    const std::u32string& NamespaceUri() const { return namespaceUri; }
    const std::u32string& LocalName() const { return localName; }
    const std::u32string& QualifiedName() const { return qualifiedName; }
    const std::u32string& Value() const { return value; }
private:
    std::u32string namespaceUri;
    std::u32string localName;
    std::u32string qualifiedName;
    std::u32string value;
};

class Attributes
{
public:
    typedef std::vector<Attribute>::const_iterator const_iterator;
    typedef std::vector<Attribute>::iterator iterator;
    iterator begin() { return attributes.begin(); }
    iterator end() { return attributes.end(); }
    const_iterator begin() const { return attributes.cbegin(); }
    const_iterator end() const { return attributes.cend(); }
    const_iterator cbegin() const { return attributes.cbegin(); }
    const_iterator cend() const { return attributes.cend(); }
    void Add(const Attribute& attribute);
    void Clear();
    size_t Count() const { return attributes.size(); }
    const std::u32string* GetAttributeValue(const std::u32string& namespaceUri, const std::u32string& localName) const;
    const std::u32string* GetAttributeValue(const std::u32string& qualifiedName) const;
private:
    std::vector<Attribute> attributes;
};

class XmlContentHandler
{
public:
    virtual ~XmlContentHandler();
    virtual void StartDocument() {}
    virtual void EndDocument() {}
    virtual void Version(const std::u32string& xmlVersion) {}
    virtual void Standalone(bool standalone) {}
    virtual void Encoding(const std::u32string& encoding) {}
    virtual void Text(const std::u32string& text) {}
    virtual void Comment(const std::u32string& comment) {}
    virtual void PI(const std::u32string& target, const std::u32string& data) {}
    virtual void CDataSection(const std::u32string& cdata) {}
    virtual void StartElement(const std::u32string& namespaceUri, const std::u32string& localName, const std::u32string& qualifiedName, const Attributes& attributes, 
        const lexer::SourcePos& sourcePos) {}
    virtual void EndElement(const std::u32string& namespaceUri, const std::u32string& localName, const std::u32string& qualifiedName) {}
    virtual void SkippedEntity(const std::u32string& entityName) {}
};

} // namespace xml

