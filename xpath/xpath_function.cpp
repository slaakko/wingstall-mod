// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module xpath.xpath_function;

import xpath.xpath_context;
import dom;
import util;

namespace xpath {

struct ElementAndDocumentStringValueComputer : public xml::Visitor
{
    void Visit(xml::Text& text) override
    {
        stringValue.append(text.Data());
    }
    std::string stringValue;
};

std::string StringValue(xml::Node* node)
{
    switch (node->Kind())
    {
    case xml::NodeKind::documentNode:
    case xml::NodeKind::elementNode:
    {
        ElementAndDocumentStringValueComputer stringValueComputer;
        node->Accept(stringValueComputer);
        return stringValueComputer.stringValue;
    }
    case xml::NodeKind::attributeNode:
    {
        xml::AttributeNode* attr = static_cast<xml::AttributeNode*>(node);
        return attr->Value();
    }
    case xml::NodeKind::processingInstructionNode:
    {
        xml::ProcessingInstruction* pi = static_cast<xml::ProcessingInstruction*>(node);
        return pi->Data();
    }
    case xml::NodeKind::commentNode:
    {
        xml::Comment* comment = static_cast<xml::Comment*>(node);
        return comment->Data();
    }
    case xml::NodeKind::textNode:
    {
        xml::Text* text = static_cast<xml::Text*>(node);
        return text->Data();
    }
    case xml::NodeKind::cdataSectionNode:
    {
        xml::CDataSection* cdataSection = static_cast<xml::CDataSection*>(node);
        return cdataSection->Data();
    }
    }
    return std::string();
}

XPathFunction::XPathFunction(const std::string& name_, int minArity_, int maxArity_) : name(name_), minArity(minArity_), maxArity(maxArity_)
{
}

XPathFunction::~XPathFunction()
{
}

class XPathBooleanFunction : public XPathFunction
{
public:
    XPathBooleanFunction();
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context, std::vector<XPathObject*>& arguments) override;
};

XPathBooleanFunction::XPathBooleanFunction() : XPathFunction("boolean", 1, 1)
{
}

std::unique_ptr<XPathObject> XPathBooleanFunction::Evaluate(XPathContext& context, std::vector<XPathObject*>& arguments)
{
    if (arguments.size() != 1)
    {
        throw std::runtime_error("sngxml::xpath::boolean() function requires one argument");
    }
    XPathObject* argument = arguments[0];
    switch (argument->Type())
    {
    case XPathObjectType::boolean:
    {
        XPathBoolean* arg = static_cast<XPathBoolean*>(argument);
        return std::unique_ptr<XPathObject>(new XPathBoolean(arg->Value()));
    }
    case XPathObjectType::number:
    {
        XPathNumber* arg = static_cast<XPathNumber*>(argument);
        return std::unique_ptr<XPathObject>(new XPathBoolean(arg->Value() != 0));
    }
    case XPathObjectType::nodeSet:
    {
        XPathNodeSet* arg = static_cast<XPathNodeSet*>(argument);
        return std::unique_ptr<XPathObject>(new XPathBoolean(arg->Length() != 0));
    }
    case XPathObjectType::string:
    {
        XPathString* arg = static_cast<XPathString*>(argument);
        return std::unique_ptr<XPathObject>(new XPathBoolean(!arg->Value().empty()));
    }
    }
    return std::unique_ptr<XPathObject>(new XPathBoolean(false));
}

class XPathNumberFunction : public XPathFunction
{
public:
    XPathNumberFunction();
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context, std::vector<XPathObject*>& arguments) override;
};

XPathNumberFunction::XPathNumberFunction() : XPathFunction("number", 0, 1)
{
}

std::unique_ptr<XPathObject> XPathNumberFunction::Evaluate(XPathContext& context, std::vector<XPathObject*>& arguments)
{
    XPathObject* argument = nullptr;
    XPathNodeSet nodeSet;
    if (arguments.empty())
    {
        nodeSet.Add(context.Node());
        argument = &nodeSet;
    }
    else
    {
        if (arguments.size() != 1)
        {
            throw std::runtime_error("sngxml::xpath::number() function requires zero or one arguments");
        }
        argument = arguments[0];
    }
    switch (argument->Type())
    {
    case XPathObjectType::number:
    {
        XPathNumber* number = static_cast<XPathNumber*>(argument);
        return std::unique_ptr<XPathObject>(new XPathNumber(number->Value()));
    }
    case XPathObjectType::string:
    {
        XPathString* string = static_cast<XPathString*>(argument);
        double result = std::stod(util::Trim(string->Value()));
        return std::unique_ptr<XPathObject>(new XPathNumber(result));
    }
    case XPathObjectType::boolean:
    {
        XPathBoolean* boolean = static_cast<XPathBoolean*>(argument);
        double result = 0;
        if (boolean->Value())
        {
            result = 1;
        }
        return std::unique_ptr<XPathObject>(new XPathNumber(result));
    }
    case XPathObjectType::nodeSet:
    {
        XPathFunction* stringFunction = GetXPathLibraryFunction("string");
        std::vector<XPathObject*> stringArgs;
        stringArgs.push_back(argument);
        std::unique_ptr<XPathObject> asString = stringFunction->Evaluate(context, stringArgs);
        if (asString->Type() != XPathObjectType::string)
        {
            throw std::runtime_error("string result expected");
        }
        XPathString* string = static_cast<XPathString*>(asString.get());
        double result = std::stod(util::Trim(string->Value()));
        return std::unique_ptr<XPathObject>(new XPathNumber(result));
    }
    }
    throw std::runtime_error("invalid argument to sngxml::xpath::number() function");
    return std::unique_ptr<XPathObject>();
}

class XPathStringFunction : public XPathFunction
{
public:
    XPathStringFunction();
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context, std::vector<XPathObject*>& arguments) override;
};

XPathStringFunction::XPathStringFunction() : XPathFunction("string", 0, 1)
{
}

std::unique_ptr<XPathObject> XPathStringFunction::Evaluate(XPathContext& context, std::vector<XPathObject*>& arguments)
{
    XPathObject* argument = nullptr;
    XPathNodeSet nodeSet;
    if (arguments.empty())
    {
        nodeSet.Add(context.Node());
        argument = &nodeSet;
    }
    else
    {
        if (arguments.size() != 1)
        {
            throw std::runtime_error("sngxml::xpath::string() function requires zero or one arguments");
        }
        argument = arguments[0];
    }
    switch (argument->Type())
    {
    case XPathObjectType::nodeSet:
    {
        XPathNodeSet* nodeSet = static_cast<XPathNodeSet*>(argument);
        if (nodeSet->Length() == 0)
        {
            return std::unique_ptr<XPathObject>(new XPathString(std::string()));
        }
        else
        {
            xml::Node* node = (*nodeSet)[0];
            return std::unique_ptr<XPathObject>(new XPathString(StringValue(node)));
        }
    }
    case XPathObjectType::number:
    {
        XPathNumber* number = static_cast<XPathNumber*>(argument);
        return std::unique_ptr<XPathObject>(new XPathString(std::to_string(number->Value())));
    }
    case XPathObjectType::boolean:
    {
        XPathBoolean* boolean = static_cast<XPathBoolean*>(argument);
        std::string val = "true";
        if (!boolean->Value())
        {
            val = "false";
        }
        return std::unique_ptr<XPathObject>(new XPathString(val));
    }
    case XPathObjectType::string:
    {
        XPathString* string = static_cast<XPathString*>(argument);
        return std::unique_ptr<XPathObject>(new XPathString(string->Value()));
    }
    }
    throw std::runtime_error("invalid argument to xpath::string() function");
    return std::unique_ptr<XPathObject>();
}

class XPathLastFunction : public XPathFunction
{
public:
    XPathLastFunction();
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context, std::vector<XPathObject*>& arguments) override;
};

XPathLastFunction::XPathLastFunction() : XPathFunction("last", 0, 0)
{
}

std::unique_ptr<XPathObject> XPathLastFunction::Evaluate(XPathContext& context, std::vector<XPathObject*>& arguments)
{
    if (!arguments.empty())
    {
        throw std::runtime_error("xpath::last() function requires no arguments");
    }
    return std::unique_ptr<XPathObject>(new XPathNumber(context.Size()));
}

class XPathPositionFunction : public XPathFunction
{
public:
    XPathPositionFunction();
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context, std::vector<XPathObject*>& arguments) override;
};

XPathPositionFunction::XPathPositionFunction() : XPathFunction("position", 0, 0)
{
}

std::unique_ptr<XPathObject> XPathPositionFunction::Evaluate(XPathContext& context, std::vector<XPathObject*>& arguments)
{
    if (!arguments.empty())
    {
        throw std::runtime_error("xpath::position() function requires no arguments");
    }
    return std::unique_ptr<XPathObject>(new XPathNumber(context.Position()));
}

class XPathCountFunction : public XPathFunction
{
public:
    XPathCountFunction();
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context, std::vector<XPathObject*>& arguments) override;
};

XPathCountFunction::XPathCountFunction() : XPathFunction("count", 1, 1)
{
}

std::unique_ptr<XPathObject> XPathCountFunction::Evaluate(XPathContext& context, std::vector<XPathObject*>& arguments)
{
    if (arguments.size() != 1)
    {
        throw std::runtime_error("xpath::count() function requires one node-set argument");
    }
    else if (arguments.size() == 1)
    {
        XPathObject* arg = arguments[0];
        if (arg->Type() != XPathObjectType::nodeSet)
        {
            throw std::runtime_error("xpath::count() function requires one node-set argument");
        }
    }
    XPathNodeSet* nodeSet = static_cast<XPathNodeSet*>(arguments[0]);
    return std::unique_ptr<XPathObject>(new XPathNumber(nodeSet->Length()));
}

class XPathFunctionLibrary
{
public:
    static void Init();
    static void Done();
    static XPathFunctionLibrary& Instance() { return *instance; }
    XPathFunction* GetFunction(const std::string& functionName);
private:
    static std::unique_ptr<XPathFunctionLibrary> instance;
    std::unordered_map<std::string, XPathFunction*> functionMap;
    std::vector<std::unique_ptr<XPathFunction>> functions;
    XPathFunctionLibrary();
};

std::unique_ptr<XPathFunctionLibrary> XPathFunctionLibrary::instance;

void XPathFunctionLibrary::Init()
{
    instance.reset(new XPathFunctionLibrary());
}

void XPathFunctionLibrary::Done()
{
    instance.reset();
}

XPathFunctionLibrary::XPathFunctionLibrary()
{
    functions.push_back(std::unique_ptr<XPathFunction>(new XPathBooleanFunction()));
    functions.push_back(std::unique_ptr<XPathFunction>(new XPathNumberFunction()));
    functions.push_back(std::unique_ptr<XPathFunction>(new XPathStringFunction()));
    functions.push_back(std::unique_ptr<XPathFunction>(new XPathLastFunction()));
    functions.push_back(std::unique_ptr<XPathFunction>(new XPathPositionFunction()));
    functions.push_back(std::unique_ptr<XPathFunction>(new XPathCountFunction()));
    for (const std::unique_ptr<XPathFunction>& function : functions)
    {
        functionMap[function->Name()] = function.get();
    }
}

XPathFunction* XPathFunctionLibrary::GetFunction(const std::string& functionName)
{
    std::unordered_map<std::string, XPathFunction*>::const_iterator it = functionMap.find(functionName);
    if (it != functionMap.cend())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("sngxml::xpath function '" + functionName + "' not found");
    }
}

XPathFunction* GetXPathLibraryFunction(const std::string& functionName)
{
    return XPathFunctionLibrary::Instance().GetFunction(functionName);
}

void InitFunction()
{
    XPathFunctionLibrary::Init();
}

void DoneFunction()
{
    XPathFunctionLibrary::Done();
}

} // namespace xpath
