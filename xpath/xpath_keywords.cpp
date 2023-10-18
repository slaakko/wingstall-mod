
// this file has been automatically generated from 'D:/work/soulng-project/sngxml/xpath/XPathLexer.lexer' using soulng lexer generator slg version 3.0.0

module xpath.xpath_keywords;

import xpath.xpath_tokens;

namespace XPathKeywords
{
    using namespace XPathTokens;

    lexer::Keyword keywords[] =
    {
        {U"or", OR},
        {U"and", AND},
        {U"div", DIV},
        {U"mod", MOD},
        {U"ancestor", ANCESTOR},
        {U"ancestor-or-self", ANCESTOR_OR_SELF},
        {U"attribute", ATTRIBUTE},
        {U"child", CHILD},
        {U"descendant", DESCENDANT},
        {U"descendant-or-self", DESCENDANT_OR_SELF},
        {U"following", FOLLOWING},
        {U"following-sibling", FOLLOWING_SIBLING},
        {U"namespace", NAMESPACE},
        {U"parent", PARENT},
        {U"preceding", PRECEDING},
        {U"preceding-sibling", PRECEDING_SIBLING},
        {U"self", SELF},
        {U"processing-instruction", PROCESSING_INSTRUCTION},
        {U"comment", COMMENT},
        {U"text", TEXT},
        {U"node", NODE},
        {nullptr, -1}
    };

    lexer::KeywordMap keywordMap(keywords);

    lexer::KeywordMap* GetKeywordMap()
    {
        return &keywordMap;
    }
}
