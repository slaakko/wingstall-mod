// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module dom.element;

import std.core;
import dom.parent_node;
import dom.attribute_node;

export namespace xml {

class Element : public ParentNode
{
public:
    Element(const lexer::SourcePos& sourcePos_, const std::string& name_);
    const std::map<std::string, std::unique_ptr<AttributeNode>>& Attributes() const { return attributeMap; }
    AttributeNode* GetAttributeNode(const std::string& attributeName) const;
    std::string GetAttribute(const std::string& name) const;
    void AddAttribute(AttributeNode* attributeNode);
    void SetAttribute(const lexer::SourcePos& sourcePos, const std::string& name, const std::string& value);
    void SetAttribute(const std::string& name, const std::string& value);
    bool HasAttributes() const final { return !attributeMap.empty(); }
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
    void WalkAttribute(NodeOperation& operation) override;
    std::string ToString();
private:
    void WriteAttributes(util::CodeFormatter& formatter);
    bool HasMultilineContent() const;
    std::map<std::string, std::unique_ptr<AttributeNode>> attributeMap;
};

Element* MakeElement(const std::string& name);

} // namespace xml
