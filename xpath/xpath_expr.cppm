// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module xpath.xpath_expr;

import xpath.xpath_object;
import xpath.xpath_context;
import dom;
import std.core;

export namespace xpath {

enum class Operator
{
    equal, notEqual, less, greater, lessOrEqual, greaterOrEqual, plus, minus, mul, div, mod, slash, slashSlash
};

class XPathExpr
{
public:
    XPathExpr();
    XPathExpr(const XPathExpr&) = delete;
    XPathExpr& operator=(const XPathExpr&) = delete;
    XPathExpr(XPathExpr&&) = delete;
    XPathExpr&& operator=(XPathExpr&&) = delete;
    virtual ~XPathExpr();
    virtual std::unique_ptr<XPathObject> Evaluate(XPathContext& context) { return std::unique_ptr<XPathObject>(); }
    virtual std::string TextValue() const { return std::string(); }
};

class XPathUnaryExpr : public XPathExpr
{
public:
    XPathUnaryExpr(XPathExpr* operand_);
    XPathExpr* Operand() const { return operand.get(); }
private:
    std::unique_ptr<XPathExpr> operand;
};

class XPathBinaryExpr : public XPathExpr
{
public:
    XPathBinaryExpr(XPathExpr* left_, XPathExpr* right_);
    XPathExpr* Left() const { return left.get(); }
    XPathExpr* Right() const { return right.get(); }
private:
    std::unique_ptr<XPathExpr> left;
    std::unique_ptr<XPathExpr> right;
};

class XPathOrExpr : public XPathBinaryExpr
{
public:
    XPathOrExpr(XPathExpr* left_, XPathExpr* right_);
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context) override;
};

class XPathAndExpr : public XPathBinaryExpr
{
public:
    XPathAndExpr(XPathExpr* left_, XPathExpr* right_);
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context) override;
};

class XPathEqualExpr : public XPathBinaryExpr
{
public:
    XPathEqualExpr(XPathExpr* left_, XPathExpr* right_);
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context) override;
};

class XPathNotEqualExpr : public XPathBinaryExpr
{
public:
    XPathNotEqualExpr(XPathExpr* left_, XPathExpr* right_);
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context) override;
};

class XPathLessExpr : public XPathBinaryExpr
{
public:
    XPathLessExpr(XPathExpr* left_, XPathExpr* right_);
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context) override;
};

class XPathGreaterExpr : public XPathBinaryExpr
{
public:
    XPathGreaterExpr(XPathExpr* left_, XPathExpr* right_);
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context) override;
};

class XPathLessOrEqualExpr : public XPathBinaryExpr
{
public:
    XPathLessOrEqualExpr(XPathExpr* left_, XPathExpr* right_);
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context) override;
};

class XPathGreaterOrEqualExpr : public XPathBinaryExpr
{
public:
    XPathGreaterOrEqualExpr(XPathExpr* left_, XPathExpr* right_);
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context) override;
};

class XPathAddExpr : public XPathBinaryExpr
{
public:
    XPathAddExpr(XPathExpr* left_, XPathExpr* right_);
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context) override;
};

class XPathSubExpr : public XPathBinaryExpr
{
public:
    XPathSubExpr(XPathExpr* left_, XPathExpr* right_);
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context) override;
};

class XPathMulExpr : public XPathBinaryExpr
{
public:
    XPathMulExpr(XPathExpr* left_, XPathExpr* right_);
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context) override;
};

class XPathDivExpr : public XPathBinaryExpr
{
public:
    XPathDivExpr(XPathExpr* left_, XPathExpr* right_);
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context) override;
};

class XPathModExpr : public XPathBinaryExpr
{
public:
    XPathModExpr(XPathExpr* left_, XPathExpr* right_);
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context) override;
};

class XPathUnaryMinusExpr : public XPathUnaryExpr
{
public:
    XPathUnaryMinusExpr(XPathExpr* operand_);
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context) override;
};

class XPathUnionExpr : public XPathBinaryExpr
{
public:
    XPathUnionExpr(XPathExpr* left_, XPathExpr* right_);
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context) override;
};

class XPathCombineStepExpr : public XPathBinaryExpr
{
public:
    XPathCombineStepExpr(XPathExpr* left_, XPathExpr* right_);
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context) override;
};

class XPathRootNodeExpr : public XPathExpr
{
public:
    XPathRootNodeExpr();
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context) override;
};

class XPathFilterExpr : public XPathUnaryExpr
{
public:
    XPathFilterExpr(XPathExpr* expr_, XPathExpr* predicate_);
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context) override;
private:
    std::unique_ptr<XPathExpr> predicate;
};

class XPathNodeTestExpr : public XPathExpr
{
public:
    virtual bool Select(xml::Node* node, xml::Axis axis) const { return true; }
};

class XPathLocationStepExpr : public XPathExpr
{
public:
    XPathLocationStepExpr(xml::Axis axis_, XPathNodeTestExpr* nodeTest_);
    XPathLocationStepExpr(const XPathLocationStepExpr&) = delete;
    XPathLocationStepExpr& operator=(const XPathLocationStepExpr&) = delete;
    XPathLocationStepExpr(XPathLocationStepExpr&&) = delete;
    XPathLocationStepExpr&& operator=(XPathLocationStepExpr&&) = delete;
    void AddPredicate(XPathExpr* predicate);
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context) override;
private:
    xml::Axis axis;
    std::unique_ptr<XPathNodeTestExpr> nodeTest;
    std::vector<std::unique_ptr<XPathExpr>> predicates;
};

xml::Axis GetAxis(const std::string& axisName);

class XPathPILiteralTest : public XPathNodeTestExpr
{
public:
    XPathPILiteralTest(XPathExpr* literal_);
    bool Select(xml::Node* node, xml::Axis axis) const override;
private:
    std::unique_ptr<XPathExpr> literal;
};

class XPathCommentNodeTest : public XPathNodeTestExpr
{
public:
    bool Select(xml::Node* node, xml::Axis axis) const override;
};

class XPathTextNodeTest : public XPathNodeTestExpr
{
public:
    bool Select(xml::Node* node, xml::Axis axis) const override;
};

class XPathPINodeTest : public XPathNodeTestExpr
{
public:
    bool Select(xml::Node* node, xml::Axis axis) const override;
};

class XPathAnyNodeTest : public XPathNodeTestExpr
{
public:
    bool Select(xml::Node* node, xml::Axis axis) const override;
};

class XPathPrincipalNodeTest : public XPathNodeTestExpr
{
public:
    bool Select(xml::Node* node, xml::Axis axis) const override;
};

class XPathPrefixTest : public XPathNodeTestExpr
{
public:
    XPathPrefixTest(const std::string& name_);
    bool Select(xml::Node* node, xml::Axis axis) const override;
private:
    std::string name;
};

class XPathNameTest : public XPathNodeTestExpr
{
public:
    XPathNameTest(const std::string& name_);
    bool Select(xml::Node* node, xml::Axis axis) const override;
private:
    std::string name;
};

class XPathVariableReference : public XPathExpr
{
public:
    XPathVariableReference(const std::string& name_);
private:
    std::string name;
};

class XPathLiteral : public XPathExpr
{
public:
    XPathLiteral(const std::string& value_);
    std::string TextValue() const override { return value; }
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context) override;
private:
    std::string value;
};

class XPathNumberExpr : public XPathExpr
{
public:
    XPathNumberExpr(const std::string& value_);
    XPathNumberExpr(double value_);
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context) override;
private:
    double value;
};

class XPathFunctionCall : public XPathExpr
{
public:
    XPathFunctionCall(const std::string& functionName_);
    void AddArgument(XPathExpr* argument);
    std::unique_ptr<XPathObject> Evaluate(XPathContext& context) override;
private:
    std::string functionName;
    std::vector<std::unique_ptr<XPathExpr>> arguments;
};

} // namespace xpath
