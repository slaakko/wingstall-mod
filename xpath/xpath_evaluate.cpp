// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module xpath.xpath_evaluate;

import xpath.xpath_context;
import xpath.xpath_parser;
import xpath.xpath_lexer;
import xpath.xpath_expr;
import util;

namespace xpath {

std::unique_ptr<XPathObject> Evaluate(const std::string& xpathExpression, xml::Node* node)
{
    XPathLexer xpathLexer(util::ToUtf32(xpathExpression), "", 0);
    std::unique_ptr<XPathExpr> xpathExpr(XPathParser::Parse(xpathLexer));
    XPathContext context(node, 1, 1);
    std::unique_ptr<XPathObject> result = xpathExpr->Evaluate(context);
    return result;
}

std::unique_ptr<XPathObject> Evaluate(const std::string& xpathExpression, xml::Document* document)
{
    return Evaluate(xpathExpression, static_cast<xml::Node*>(document));
}

} // namespace xpath
