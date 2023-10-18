// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module xpath.xpath_object;

import dom;
import util;

namespace xpath {

XPathObject::XPathObject(XPathObjectType type_) : type(type_)
{
}

XPathObject::~XPathObject()
{
}

XPathNodeSet::XPathNodeSet() : XPathObject(XPathObjectType::nodeSet)
{
}

void XPathNodeSet::Add(xml::Node* node)
{
    nodes.push_back(node);
}

XPathBoolean::XPathBoolean(bool value_) : XPathObject(XPathObjectType::boolean), value(value_)
{
}

XPathNumber::XPathNumber(double value_) : XPathObject(XPathObjectType::number), value(value_)
{
}

XPathString::XPathString(const std::string& value_) : XPathObject(XPathObjectType::string), value(value_)
{
}

} // namespace xpath
