// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module xpath.xpath_context;

namespace xpath {

XPathContext::XPathContext(xml::Node* node_, int position_, int size_) : node(node_), position(position_), size(size_)
{
}

} // namespace xpath
