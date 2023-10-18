// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module xpath.xpath_context;

import dom;
import std.core;

export namespace xpath {

class XPathContext
{
public:
    XPathContext(xml::Node* node_, int position_, int size_);
    xml::Node* Node() const { return node; }
    int Position() const { return position; }
    int Size() const { return size; }
private:
    xml::Node* node;
    int position;
    int size;
};

} // namespace xpath
