// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module dom.attribute_node;

import std.core;
import dom.node;

export namespace xml {

std::string AttrValueEscape(const std::string& attributeValue, char delimiter);
std::string MakeXmlAttrValue(const std::string& value);

class AttributeNode : public Node
{
public:
    AttributeNode(const lexer::SourcePos& sourcePos_, const std::string& name_, const std::string& value_);
    const std::string& Value() const { return value; }
    void SetValue(const std::string& value_);
    void Write(util::CodeFormatter& formatter) override;
private:
    std::string value;
};

AttributeNode* MakeAttribute(const std::string& name, const std::string& value);

} // namespace xml
