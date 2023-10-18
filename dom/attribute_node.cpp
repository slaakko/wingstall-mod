// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module dom.attribute_node;

import util;

namespace xml {

std::string AttrValueEscape(const std::string& attributeValue, char delimiter)
{
    std::string result;
    std::u32string attValue = util::ToUtf32(attributeValue);
    for (char32_t c : attValue)
    {
        switch (c)
        {
        case '<': result.append("&lt;"); break;
        case '&': result.append("&amp;"); break;
        case '"': if (delimiter == '"') result.append("&quot;"); else result.append(1, '"'); break;
        case '\'': if (delimiter == '\'') result.append("&apos;"); else result.append(1, '\''); break;
        default:
        {
            if (c >= 32 && c < 127)
            {
                result.append(1, static_cast<char>(c));
            }
            else
            {
                int codePoint = static_cast<int>(c);
                std::string charText = "&#";
                charText.append(std::to_string(codePoint)).append(";");
                result.append(charText);
            }
            break;
        }
        }
    }
    return result;
}

std::string MakeXmlAttrValue(const std::string& value)
{
    std::string result;
    if (value.find('"') == std::string::npos)
    {
        result.append(1, '"');
        result.append(AttrValueEscape(value, '"'));
        result.append(1, '"');
    }
    else if (value.find('\'') == std::string::npos)
    {
        result.append(1, '\'');
        result.append(AttrValueEscape(value, '\''));
        result.append(1, '\'');
    }
    else
    {
        result.append(1, '"');
        result.append(AttrValueEscape(value, '"'));
        result.append(1, '"');
    }
    return result;
}

AttributeNode::AttributeNode(const lexer::SourcePos& sourcePos_, const std::string& name_, const std::string& value_) :
    Node(NodeKind::attributeNode, sourcePos_, name_), value(value_)
{
}

void AttributeNode::SetValue(const std::string& value_)
{
    value = value_;
}

void AttributeNode::Write(util::CodeFormatter& formatter)
{
    formatter.Write(" " + Name() + "=");
    formatter.Write(MakeXmlAttrValue(value));
}

AttributeNode* MakeAttribute(const std::string& name, const std::string& value)
{
    return new AttributeNode(lexer::SourcePos(), name, value);
}

} // namespace xml
