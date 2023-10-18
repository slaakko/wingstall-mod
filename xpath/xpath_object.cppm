// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module xpath.xpath_object;

import dom;
import std.core;

export namespace xpath {

enum class XPathObjectType
{
    nodeSet, boolean, number, string
};

class XPathObject
{
public:
    XPathObject(XPathObjectType type_);
    XPathObjectType Type() const { return type; }
    virtual ~XPathObject();
private:
    XPathObjectType type;
};

class XPathNodeSet : public XPathObject
{
public:
    XPathNodeSet();
    xml::Node* operator[](int index) const { return nodes[index]; }
    int Length() const { return nodes.size(); }
    void Add(xml::Node* node);
private:
    std::vector<xml::Node*> nodes;
};

class XPathBoolean : public XPathObject
{
public:
    XPathBoolean(bool value_);
    bool Value() const { return value; }
private:
    bool value;
};

class XPathNumber : public XPathObject
{
public:
    XPathNumber(double value_);
    double Value() const { return value; }
private:
    double value;
};

class XPathString : public XPathObject
{
public:
    XPathString(const std::string& value_);
    const std::string& Value() const { return value; }
private:
    std::string value;
};

} // namespace xpath

