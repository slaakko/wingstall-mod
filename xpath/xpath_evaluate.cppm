// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module xpath.xpath_evaluate;

import dom;
import xpath.xpath_object;
import std.core;

export namespace xpath {

std::unique_ptr<XPathObject> Evaluate(const std::string& xpathExpression, xml::Node* node);
std::unique_ptr<XPathObject> Evaluate(const std::string& xpathExpression, xml::Document* document);

} // namespace xpath
