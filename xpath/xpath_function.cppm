// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module xpath.xpath_function;

import dom;
import xpath.xpath_object;
import std.core;

export namespace xpath {

class XPathContext;

std::string StringValue(xml::Node* node);

class XPathFunction
{
public:
    XPathFunction(const std::string& name_, int minArity_, int maxArity_);
    virtual ~XPathFunction();
    virtual std::unique_ptr<XPathObject> Evaluate(XPathContext& context, std::vector<XPathObject*>& arguments) = 0;
    const std::string& Name() const { return name; }
    int MinArity() const { return minArity; }
    int MaxArity() const { return maxArity; }
private:
    std::string name;
    int minArity;
    int maxArity;
};

XPathFunction* GetXPathLibraryFunction(const std::string& functionName);

void InitFunction();
void DoneFunction();

} // namespace xpath
