// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module dom.character_data;

import util;

namespace xml {

std::string XmlCharDataEscape(const std::string& text)
{
    std::string result;
    std::u32string value = util::ToUtf32(text);
    for (char32_t c : value)
    {
        switch (c)
        {
        case '<': result.append("&lt;"); break;
        case '&': result.append("&amp;"); break;
        case '\r': case '\n': result.append(1, static_cast<char>(c)); break;
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

CharacterData::CharacterData(NodeKind kind_, const lexer::SourcePos& sourcePos_, const std::string& name_) :
    CharacterData(kind_, sourcePos_, name_, std::string())
{
}

CharacterData::CharacterData(NodeKind kind_, const lexer::SourcePos& sourcePos_, const std::string& name_, const std::string& data_) :
    Node(kind_, sourcePos_, name_), data(data_)
{
}

void CharacterData::Write(util::CodeFormatter& formatter)
{
    formatter.Write(XmlCharDataEscape(data));
}

bool CharacterData::ValueContainsNewLine() const
{
    return data.find('\n') != std::string::npos;
}

} // namespace xml
