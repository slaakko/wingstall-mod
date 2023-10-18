// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module xpath.xpath_expr;

import xpath.xpath_function;
import util;

namespace xpath {

XPathExpr::XPathExpr()
{
}

XPathExpr::~XPathExpr()
{
}

XPathUnaryExpr::XPathUnaryExpr(XPathExpr* operand_) : operand(operand_)
{
}

XPathBinaryExpr::XPathBinaryExpr(XPathExpr* left_, XPathExpr* right_) : left(left_), right(right_)
{
}

XPathOrExpr::XPathOrExpr(XPathExpr* left_, XPathExpr* right_) : XPathBinaryExpr(left_, right_)
{
}

std::unique_ptr<XPathObject> XPathOrExpr::Evaluate(XPathContext& context)
{
    XPathFunction* boolean = GetXPathLibraryFunction("boolean");
    std::unique_ptr<XPathObject> left = Left()->Evaluate(context);
    std::vector<XPathObject*> leftArgs;
    leftArgs.push_back(left.get());
    std::unique_ptr<XPathObject> leftAsBoolean = boolean->Evaluate(context, leftArgs);
    if (leftAsBoolean->Type() != XPathObjectType::boolean)
    {
        throw std::runtime_error("boolean result expected");
    }
    if (static_cast<XPathBoolean*>(leftAsBoolean.get())->Value())
    {
        return leftAsBoolean;
    }
    std::unique_ptr<XPathObject> right = Right()->Evaluate(context);
    std::vector<XPathObject*> rightArgs;
    rightArgs.push_back(right.get());
    std::unique_ptr<XPathObject> rightAsBoolean = boolean->Evaluate(context, rightArgs);
    if (rightAsBoolean->Type() != XPathObjectType::boolean)
    {
        throw std::runtime_error("boolean result expected");
    }
    return rightAsBoolean;
}

XPathAndExpr::XPathAndExpr(XPathExpr* left_, XPathExpr* right_) : XPathBinaryExpr(left_, right_)
{
}

std::unique_ptr<XPathObject> XPathAndExpr::Evaluate(XPathContext& context)
{
    XPathFunction* boolean = GetXPathLibraryFunction("boolean");
    std::unique_ptr<XPathObject> left = Left()->Evaluate(context);
    std::vector<XPathObject*> leftArgs;
    leftArgs.push_back(left.get());
    std::unique_ptr<XPathObject> leftAsBoolean = boolean->Evaluate(context, leftArgs);
    if (leftAsBoolean->Type() != XPathObjectType::boolean)
    {
        throw std::runtime_error("boolean result expected");
    }
    if (!static_cast<XPathBoolean*>(leftAsBoolean.get())->Value())
    {
        return leftAsBoolean;
    }
    std::unique_ptr<XPathObject> right = Right()->Evaluate(context);
    std::vector<XPathObject*> rightArgs;
    rightArgs.push_back(right.get());
    std::unique_ptr<XPathObject> rightAsBoolean = boolean->Evaluate(context, rightArgs);
    if (rightAsBoolean->Type() != XPathObjectType::boolean)
    {
        throw std::runtime_error("boolean result expected");
    }
    return rightAsBoolean;
}

std::unique_ptr<XPathObject> CompareNodeSets(XPathContext& context, XPathObject* left, XPathObject* right, Operator comparisonOp)
{
    if (left->Type() == XPathObjectType::nodeSet && right->Type() == XPathObjectType::nodeSet)
    {
        XPathNodeSet* leftNodeSet = static_cast<XPathNodeSet*>(left);
        XPathNodeSet* rightNodeSet = static_cast<XPathNodeSet*>(right);
        int n = leftNodeSet->Length();
        for (int i = 0; i < n; ++i)
        {
            xml::Node* leftNode = (*leftNodeSet)[i];
            std::string leftStr = StringValue(leftNode);
            int m = rightNodeSet->Length();
            for (int j = 0; j < m; ++j)
            {
                xml::Node* rightNode = (*rightNodeSet)[j];
                std::string rightStr = StringValue(rightNode);
                switch (comparisonOp)
                {
                case Operator::equal:
                {
                    if (leftStr == rightStr)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::notEqual:
                {
                    if (leftStr != rightStr)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::less:
                {
                    if (leftStr < rightStr)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::greater:
                {
                    if (leftStr > rightStr)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::lessOrEqual:
                {
                    if (leftStr <= rightStr)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::greaterOrEqual:
                {
                    if (leftStr >= rightStr)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                }
            }
        }
    }
    else if (left->Type() == XPathObjectType::nodeSet)
    {
        XPathFunction* numberFunction = GetXPathLibraryFunction("number");
        XPathFunction* booleanFunction = GetXPathLibraryFunction("boolean");
        if (right->Type() == XPathObjectType::number)
        {
            double rightNumber = static_cast<XPathNumber*>(right)->Value();
            XPathNodeSet* leftNodeSet = static_cast<XPathNodeSet*>(left);
            int n = leftNodeSet->Length();
            for (int i = 0; i < n; ++i)
            {
                xml::Node* leftNode = (*leftNodeSet)[i];
                XPathString leftAsString(StringValue(leftNode));
                std::vector<XPathObject*> leftArgs;
                leftArgs.push_back(&leftAsString);
                std::unique_ptr<XPathObject> leftAsNumber = numberFunction->Evaluate(context, leftArgs);
                if (leftAsNumber->Type() != XPathObjectType::number)
                {
                    throw std::runtime_error("number result expected");
                }
                double leftNumber = static_cast<XPathNumber*>(leftAsNumber.get())->Value();
                switch (comparisonOp)
                {
                case Operator::equal:
                {
                    if (leftNumber == rightNumber)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::notEqual:
                {
                    if (leftNumber != rightNumber)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::less:
                {
                    if (leftNumber < rightNumber)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::greater:
                {
                    if (leftNumber > rightNumber)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::lessOrEqual:
                {
                    if (leftNumber <= rightNumber)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::greaterOrEqual:
                {
                    if (leftNumber >= rightNumber)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                }
            }
        }
        else if (right->Type() == XPathObjectType::string)
        {
            std::string rightString = static_cast<XPathString*>(right)->Value();
            XPathNodeSet* leftNodeSet = static_cast<XPathNodeSet*>(left);
            int n = leftNodeSet->Length();
            for (int i = 0; i < n; ++i)
            {
                xml::Node* leftNode = (*leftNodeSet)[i];
                std::string leftString = StringValue(leftNode);
                switch (comparisonOp)
                {
                case Operator::equal:
                {
                    if (leftString == rightString)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::notEqual:
                {
                    if (leftString != rightString)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::less:
                {
                    if (leftString < rightString)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::greater:
                {
                    if (leftString > rightString)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::lessOrEqual:
                {
                    if (leftString <= rightString)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::greaterOrEqual:
                {
                    if (leftString >= rightString)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                }
            }
        }
        else if (right->Type() == XPathObjectType::boolean)
        {
            bool rightBool = static_cast<XPathBoolean*>(right)->Value();
            std::vector<XPathObject*> leftArgs;
            leftArgs.push_back(left);
            std::unique_ptr<XPathObject> leftAsBool = booleanFunction->Evaluate(context, leftArgs);
            if (leftAsBool->Type() != XPathObjectType::boolean)
            {
                throw std::runtime_error("boolean result expected");
            }
            bool leftBool = static_cast<XPathBoolean*>(leftAsBool.get())->Value();
            switch (comparisonOp)
            {
            case Operator::equal:
            {
                if (leftBool == rightBool)
                {
                    std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                    return result;
                }
                break;
            }
            case Operator::notEqual:
            {
                if (leftBool != rightBool)
                {
                    std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                    return result;
                }
                break;
            }
            case Operator::less:
            {
                if (leftBool < rightBool)
                {
                    std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                    return result;
                }
                break;
            }
            case Operator::greater:
            {
                if (leftBool > rightBool)
                {
                    std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                    return result;
                }
                break;
            }
            case Operator::lessOrEqual:
            {
                if (leftBool <= rightBool)
                {
                    std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                    return result;
                }
                break;
            }
            case Operator::greaterOrEqual:
            {
                if (leftBool >= rightBool)
                {
                    std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                    return result;
                }
                break;
            }
            }
        }
    }
    else if (right->Type() == XPathObjectType::nodeSet)
    {
        XPathFunction* numberFunction = GetXPathLibraryFunction("number");
        XPathFunction* booleanFunction = GetXPathLibraryFunction("boolean");
        if (left->Type() == XPathObjectType::number)
        {
            double leftNumber = static_cast<XPathNumber*>(left)->Value();
            XPathNodeSet* rightNodeSet = static_cast<XPathNodeSet*>(right);
            int n = rightNodeSet->Length();
            for (int i = 0; i < n; ++i)
            {
                xml::Node* rightNode = (*rightNodeSet)[i];
                XPathString rightAsString(StringValue(rightNode));
                std::vector<XPathObject*> rightArgs;
                rightArgs.push_back(&rightAsString);
                std::unique_ptr<XPathObject> rightAsNumber = numberFunction->Evaluate(context, rightArgs);
                if (rightAsNumber->Type() != XPathObjectType::number)
                {
                    throw std::runtime_error("number result expected");
                }
                double rightNumber = static_cast<XPathNumber*>(rightAsNumber.get())->Value();
                switch (comparisonOp)
                {
                case Operator::equal:
                {
                    if (leftNumber == rightNumber)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::notEqual:
                {
                    if (leftNumber != rightNumber)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::less:
                {
                    if (leftNumber < rightNumber)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::greater:
                {
                    if (leftNumber > rightNumber)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::lessOrEqual:
                {
                    if (leftNumber <= rightNumber)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::greaterOrEqual:
                {
                    if (leftNumber >= rightNumber)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                }
            }
        }
        else if (left->Type() == XPathObjectType::string)
        {
            std::string leftString = static_cast<XPathString*>(left)->Value();
            XPathNodeSet* rightNodeSet = static_cast<XPathNodeSet*>(right);
            int n = rightNodeSet->Length();
            for (int i = 0; i < n; ++i)
            {
                xml::Node* rightNode = (*rightNodeSet)[i];
                std::string rightString = StringValue(rightNode);
                switch (comparisonOp)
                {
                case Operator::equal:
                {
                    if (leftString == rightString)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::notEqual:
                {
                    if (leftString != rightString)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::less:
                {
                    if (leftString < rightString)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::greater:
                {
                    if (leftString > rightString)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::lessOrEqual:
                {
                    if (leftString <= rightString)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                case Operator::greaterOrEqual:
                {
                    if (leftString >= rightString)
                    {
                        std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                        return result;
                    }
                    break;
                }
                }
            }
        }
        else if (left->Type() == XPathObjectType::boolean)
        {
            bool leftBool = static_cast<XPathBoolean*>(left)->Value();
            std::vector<XPathObject*> rightArgs;
            rightArgs.push_back(right);
            std::unique_ptr<XPathObject> rightAsBool = booleanFunction->Evaluate(context, rightArgs);
            if (rightAsBool->Type() != XPathObjectType::boolean)
            {
                throw std::runtime_error("boolean result expected");
            }
            bool rightBool = static_cast<XPathBoolean*>(rightAsBool.get())->Value();
            switch (comparisonOp)
            {
            case Operator::equal:
            {
                if (leftBool == rightBool)
                {
                    std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                    return result;
                }
                break;
            }
            case Operator::notEqual:
            {
                if (leftBool != rightBool)
                {
                    std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                    return result;
                }
                break;
            }
            case Operator::less:
            {
                if (leftBool < rightBool)
                {
                    std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                    return result;
                }
                break;
            }
            case Operator::greater:
            {
                if (leftBool > rightBool)
                {
                    std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                    return result;
                }
                break;
            }
            case Operator::lessOrEqual:
            {
                if (leftBool <= rightBool)
                {
                    std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                    return result;
                }
                break;
            }
            case Operator::greaterOrEqual:
            {
                if (leftBool >= rightBool)
                {
                    std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                    return result;
                }
                break;
            }
            }
        }
    }
    std::unique_ptr<XPathObject> result(new XPathBoolean(false));
    return result;
}

std::unique_ptr<XPathObject> CompareEquality(XPathContext& context, XPathObject* left, XPathObject* right)
{
    if (left->Type() == XPathObjectType::nodeSet || right->Type() == XPathObjectType::nodeSet)
    {
        return CompareNodeSets(context, left, right, Operator::equal);
    }
    else
    {
        XPathFunction* numberFunction = GetXPathLibraryFunction("number");
        XPathFunction* booleanFunction = GetXPathLibraryFunction("boolean");
        XPathFunction* stringFunction = GetXPathLibraryFunction("string");
        if (left->Type() == XPathObjectType::number || right->Type() == XPathObjectType::number)
        {
            std::vector<XPathObject*> leftArgs;
            leftArgs.push_back(left);
            std::unique_ptr<XPathObject> leftAsNumber = numberFunction->Evaluate(context, leftArgs);
            if (leftAsNumber->Type() != XPathObjectType::number)
            {
                throw std::runtime_error("number result expected");
            }
            double leftNumber = static_cast<XPathNumber*>(leftAsNumber.get())->Value();
            std::vector<XPathObject*> rightArgs;
            rightArgs.push_back(right);
            std::unique_ptr<XPathObject> rightAsNumber = numberFunction->Evaluate(context, rightArgs);
            if (rightAsNumber->Type() != XPathObjectType::number)
            {
                throw std::runtime_error("number result expected");
            }
            double rightNumber = static_cast<XPathNumber*>(rightAsNumber.get())->Value();
            if (leftNumber == rightNumber)
            {
                std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                return result;
            }
            else
            {
                std::unique_ptr<XPathObject> result(new XPathBoolean(false));
                return result;
            }
        }
        else
        {
            std::vector<XPathObject*> leftArgs;
            leftArgs.push_back(left);
            std::unique_ptr<XPathObject> leftAsString = stringFunction->Evaluate(context, leftArgs);
            if (leftAsString->Type() != XPathObjectType::string)
            {
                throw std::runtime_error("string result expected");
            }
            std::string leftString = static_cast<XPathString*>(leftAsString.get())->Value();
            std::vector<XPathObject*> rightArgs;
            rightArgs.push_back(right);
            std::unique_ptr<XPathObject> rightAsString = stringFunction->Evaluate(context, rightArgs);
            if (rightAsString->Type() != XPathObjectType::string)
            {
                throw std::runtime_error("string result expected");
            }
            std::string rightString = static_cast<XPathString*>(rightAsString.get())->Value();
            if (leftString == rightString)
            {
                std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                return result;
            }
            else
            {
                std::unique_ptr<XPathObject> result(new XPathBoolean(false));
                return result;
            }
        }
    }
    std::unique_ptr<XPathObject> result(new XPathBoolean(false));
    return result;
}

XPathEqualExpr::XPathEqualExpr(XPathExpr* left_, XPathExpr* right_) : XPathBinaryExpr(left_, right_)
{
}

std::unique_ptr<XPathObject> XPathEqualExpr::Evaluate(XPathContext& context)
{
    std::unique_ptr<XPathObject> left = Left()->Evaluate(context);
    std::unique_ptr<XPathObject> right = Right()->Evaluate(context);
    return CompareEquality(context, left.get(), right.get());
}

XPathNotEqualExpr::XPathNotEqualExpr(XPathExpr* left_, XPathExpr* right_) : XPathBinaryExpr(left_, right_)
{
}

std::unique_ptr<XPathObject> XPathNotEqualExpr::Evaluate(XPathContext& context)
{
    std::unique_ptr<XPathObject> left = Left()->Evaluate(context);
    std::unique_ptr<XPathObject> right = Right()->Evaluate(context);
    std::unique_ptr<XPathObject> equal = CompareEquality(context, left.get(), right.get());
    if (equal->Type() != XPathObjectType::boolean)
    {
        throw std::runtime_error("boolean result expected");
    }
    return std::unique_ptr<XPathObject>(new XPathBoolean(!static_cast<XPathBoolean*>(equal.get())->Value()));
}

std::unique_ptr<XPathObject> Compare(XPathContext& context, XPathObject* left, XPathObject* right, Operator comparisonOp)
{
    if (left->Type() == XPathObjectType::nodeSet || right->Type() == XPathObjectType::nodeSet)
    {
        return CompareNodeSets(context, left, right, comparisonOp);
    }
    else
    {
        XPathFunction* numberFunction = GetXPathLibraryFunction("number");
        std::vector<XPathObject*> leftArgs;
        leftArgs.push_back(left);
        std::unique_ptr<XPathObject> leftAsNumber = numberFunction->Evaluate(context, leftArgs);
        if (leftAsNumber->Type() != XPathObjectType::number)
        {
            throw std::runtime_error("number result expected");
        }
        double leftNumber = static_cast<XPathNumber*>(leftAsNumber.get())->Value();
        std::vector<XPathObject*> rightArgs;
        rightArgs.push_back(right);
        std::unique_ptr<XPathObject> rightAsNumber = numberFunction->Evaluate(context, rightArgs);
        if (rightAsNumber->Type() != XPathObjectType::number)
        {
            throw std::runtime_error("number result expected");
        }
        double rightNumber = static_cast<XPathNumber*>(rightAsNumber.get())->Value();
        switch (comparisonOp)
        {
        case Operator::less:
        {
            if (leftNumber < rightNumber)
            {
                std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                return result;
            }
            else
            {
                std::unique_ptr<XPathObject> result(new XPathBoolean(false));
                return result;
            }
            break;
        }
        case Operator::greater:
        {
            if (leftNumber > rightNumber)
            {
                std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                return result;
            }
            else
            {
                std::unique_ptr<XPathObject> result(new XPathBoolean(false));
                return result;
            }
            break;
        }
        case Operator::lessOrEqual:
        {
            if (leftNumber <= rightNumber)
            {
                std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                return result;
            }
            else
            {
                std::unique_ptr<XPathObject> result(new XPathBoolean(false));
                return result;
            }
            break;
        }
        case Operator::greaterOrEqual:
        {
            if (leftNumber >= rightNumber)
            {
                std::unique_ptr<XPathObject> result(new XPathBoolean(true));
                return result;
            }
            else
            {
                std::unique_ptr<XPathObject> result(new XPathBoolean(false));
                return result;
            }
            break;
        }
        }
    }
    std::unique_ptr<XPathObject> result(new XPathBoolean(false));
    return result;
}

XPathLessExpr::XPathLessExpr(XPathExpr* left_, XPathExpr* right_) : XPathBinaryExpr(left_, right_)
{
}

std::unique_ptr<XPathObject> XPathLessExpr::Evaluate(XPathContext& context)
{
    std::unique_ptr<XPathObject> left = Left()->Evaluate(context);
    std::unique_ptr<XPathObject> right = Right()->Evaluate(context);
    return Compare(context, left.get(), right.get(), Operator::less);
}

XPathGreaterExpr::XPathGreaterExpr(XPathExpr* left_, XPathExpr* right_) : XPathBinaryExpr(left_, right_)
{
}

std::unique_ptr<XPathObject> XPathGreaterExpr::Evaluate(XPathContext& context)
{
    std::unique_ptr<XPathObject> left = Left()->Evaluate(context);
    std::unique_ptr<XPathObject> right = Right()->Evaluate(context);
    return Compare(context, left.get(), right.get(), Operator::greater);
}

XPathLessOrEqualExpr::XPathLessOrEqualExpr(XPathExpr* left_, XPathExpr* right_) : XPathBinaryExpr(left_, right_)
{
}

std::unique_ptr<XPathObject> XPathLessOrEqualExpr::Evaluate(XPathContext& context)
{
    std::unique_ptr<XPathObject> left = Left()->Evaluate(context);
    std::unique_ptr<XPathObject> right = Right()->Evaluate(context);
    return Compare(context, left.get(), right.get(), Operator::lessOrEqual);
}

XPathGreaterOrEqualExpr::XPathGreaterOrEqualExpr(XPathExpr* left_, XPathExpr* right_) : XPathBinaryExpr(left_, right_)
{
}

std::unique_ptr<XPathObject> XPathGreaterOrEqualExpr::Evaluate(XPathContext& context)
{
    std::unique_ptr<XPathObject> left = Left()->Evaluate(context);
    std::unique_ptr<XPathObject> right = Right()->Evaluate(context);
    return Compare(context, left.get(), right.get(), Operator::greaterOrEqual);
}

XPathAddExpr::XPathAddExpr(XPathExpr* left_, XPathExpr* right_) : XPathBinaryExpr(left_, right_)
{
}

std::unique_ptr<XPathObject> XPathAddExpr::Evaluate(XPathContext& context)
{
    std::unique_ptr<XPathObject> left = Left()->Evaluate(context);
    std::unique_ptr<XPathObject> right = Right()->Evaluate(context);
    XPathFunction* numberFunction = GetXPathLibraryFunction("number");
    std::vector<XPathObject*> leftArgs;
    leftArgs.push_back(left.get());
    std::unique_ptr<XPathObject> leftAsNumber = numberFunction->Evaluate(context, leftArgs);
    if (leftAsNumber->Type() != XPathObjectType::number)
    {
        throw std::runtime_error("number result expected");
    }
    double leftNumber = static_cast<XPathNumber*>(leftAsNumber.get())->Value();
    std::vector<XPathObject*> rightArgs;
    rightArgs.push_back(right.get());
    std::unique_ptr<XPathObject> rightAsNumber = numberFunction->Evaluate(context, rightArgs);
    if (rightAsNumber->Type() != XPathObjectType::number)
    {
        throw std::runtime_error("number result expected");
    }
    double rightNumber = static_cast<XPathNumber*>(rightAsNumber.get())->Value();
    return std::unique_ptr<XPathObject>(new XPathNumber(leftNumber + rightNumber));
}

XPathSubExpr::XPathSubExpr(XPathExpr* left_, XPathExpr* right_) : XPathBinaryExpr(left_, right_)
{
}

std::unique_ptr<XPathObject> XPathSubExpr::Evaluate(XPathContext& context)
{
    std::unique_ptr<XPathObject> left = Left()->Evaluate(context);
    std::unique_ptr<XPathObject> right = Right()->Evaluate(context);
    XPathFunction* numberFunction = GetXPathLibraryFunction("number");
    std::vector<XPathObject*> leftArgs;
    leftArgs.push_back(left.get());
    std::unique_ptr<XPathObject> leftAsNumber = numberFunction->Evaluate(context, leftArgs);
    if (leftAsNumber->Type() != XPathObjectType::number)
    {
        throw std::runtime_error("number result expected");
    }
    double leftNumber = static_cast<XPathNumber*>(leftAsNumber.get())->Value();
    std::vector<XPathObject*> rightArgs;
    rightArgs.push_back(right.get());
    std::unique_ptr<XPathObject> rightAsNumber = numberFunction->Evaluate(context, rightArgs);
    if (rightAsNumber->Type() != XPathObjectType::number)
    {
        throw std::runtime_error("number result expected");
    }
    double rightNumber = static_cast<XPathNumber*>(rightAsNumber.get())->Value();
    return std::unique_ptr<XPathObject>(new XPathNumber(leftNumber - rightNumber));
}

XPathMulExpr::XPathMulExpr(XPathExpr* left_, XPathExpr* right_) : XPathBinaryExpr(left_, right_)
{
}

std::unique_ptr<XPathObject> XPathMulExpr::Evaluate(XPathContext& context)
{
    std::unique_ptr<XPathObject> left = Left()->Evaluate(context);
    std::unique_ptr<XPathObject> right = Right()->Evaluate(context);
    XPathFunction* numberFunction = GetXPathLibraryFunction("number");
    std::vector<XPathObject*> leftArgs;
    leftArgs.push_back(left.get());
    std::unique_ptr<XPathObject> leftAsNumber = numberFunction->Evaluate(context, leftArgs);
    if (leftAsNumber->Type() != XPathObjectType::number)
    {
        throw std::runtime_error("number result expected");
    }
    double leftNumber = static_cast<XPathNumber*>(leftAsNumber.get())->Value();
    std::vector<XPathObject*> rightArgs;
    rightArgs.push_back(right.get());
    std::unique_ptr<XPathObject> rightAsNumber = numberFunction->Evaluate(context, rightArgs);
    if (rightAsNumber->Type() != XPathObjectType::number)
    {
        throw std::runtime_error("number result expected");
    }
    double rightNumber = static_cast<XPathNumber*>(rightAsNumber.get())->Value();
    return std::unique_ptr<XPathObject>(new XPathNumber(leftNumber * rightNumber));
}

XPathDivExpr::XPathDivExpr(XPathExpr* left_, XPathExpr* right_) : XPathBinaryExpr(left_, right_)
{
}

std::unique_ptr<XPathObject> XPathDivExpr::Evaluate(XPathContext& context)
{
    std::unique_ptr<XPathObject> left = Left()->Evaluate(context);
    std::unique_ptr<XPathObject> right = Right()->Evaluate(context);
    XPathFunction* numberFunction = GetXPathLibraryFunction("number");
    std::vector<XPathObject*> leftArgs;
    leftArgs.push_back(left.get());
    std::unique_ptr<XPathObject> leftAsNumber = numberFunction->Evaluate(context, leftArgs);
    if (leftAsNumber->Type() != XPathObjectType::number)
    {
        throw std::runtime_error("number result expected");
    }
    double leftNumber = static_cast<XPathNumber*>(leftAsNumber.get())->Value();
    std::vector<XPathObject*> rightArgs;
    rightArgs.push_back(right.get());
    std::unique_ptr<XPathObject> rightAsNumber = numberFunction->Evaluate(context, rightArgs);
    if (rightAsNumber->Type() != XPathObjectType::number)
    {
        throw std::runtime_error("number result expected");
    }
    double rightNumber = static_cast<XPathNumber*>(rightAsNumber.get())->Value();
    return std::unique_ptr<XPathObject>(new XPathNumber(leftNumber / rightNumber));
}

XPathModExpr::XPathModExpr(XPathExpr* left_, XPathExpr* right_) : XPathBinaryExpr(left_, right_)
{
}

std::unique_ptr<XPathObject> XPathModExpr::Evaluate(XPathContext& context)
{
    std::unique_ptr<XPathObject> left = Left()->Evaluate(context);
    std::unique_ptr<XPathObject> right = Right()->Evaluate(context);
    XPathFunction* numberFunction = GetXPathLibraryFunction("number");
    std::vector<XPathObject*> leftArgs;
    leftArgs.push_back(left.get());
    std::unique_ptr<XPathObject> leftAsNumber = numberFunction->Evaluate(context, leftArgs);
    if (leftAsNumber->Type() != XPathObjectType::number)
    {
        throw std::runtime_error("number result expected");
    }
    int64_t leftNumber = static_cast<int64_t>(static_cast<XPathNumber*>(leftAsNumber.get())->Value());
    std::vector<XPathObject*> rightArgs;
    rightArgs.push_back(right.get());
    std::unique_ptr<XPathObject> rightAsNumber = numberFunction->Evaluate(context, rightArgs);
    if (rightAsNumber->Type() != XPathObjectType::number)
    {
        throw std::runtime_error("number result expected");
    }
    int64_t rightNumber = static_cast<int64_t>(static_cast<XPathNumber*>(rightAsNumber.get())->Value());
    return std::unique_ptr<XPathObject>(new XPathNumber(double(leftNumber % rightNumber)));
}

XPathUnaryMinusExpr::XPathUnaryMinusExpr(XPathExpr* operand_) : XPathUnaryExpr(operand_)
{
}

std::unique_ptr<XPathObject> XPathUnaryMinusExpr::Evaluate(XPathContext& context)
{
    std::unique_ptr<XPathObject> operand = Operand()->Evaluate(context);
    XPathFunction* numberFunction = GetXPathLibraryFunction("number");
    std::vector<XPathObject*> operandArgs;
    operandArgs.push_back(operand.get());
    std::unique_ptr<XPathObject> operandAsNumber = numberFunction->Evaluate(context, operandArgs);
    if (operandAsNumber->Type() != XPathObjectType::number)
    {
        throw std::runtime_error("number result expected");
    }
    double operandNumber = static_cast<XPathNumber*>(operandAsNumber.get())->Value();
    return std::unique_ptr<XPathObject>(new XPathNumber(-operandNumber));
}

XPathUnionExpr::XPathUnionExpr(XPathExpr* left_, XPathExpr* right_) : XPathBinaryExpr(left_, right_)
{
}

std::unique_ptr<XPathObject> XPathUnionExpr::Evaluate(XPathContext& context)
{
    std::unique_ptr<XPathNodeSet> result(new XPathNodeSet());
    std::unique_ptr<XPathObject> leftResult = Left()->Evaluate(context);
    if (leftResult->Type() != XPathObjectType::nodeSet)
    {
        throw std::runtime_error("node set expected");
    }
    XPathNodeSet* leftNodeSet = static_cast<XPathNodeSet*>(leftResult.get());
    int n = leftNodeSet->Length();
    for (int i = 0; i < n; ++i)
    {
        result->Add((*leftNodeSet)[i]);
    }
    std::unique_ptr<XPathObject> rightResult = Right()->Evaluate(context);
    if (rightResult->Type() != XPathObjectType::nodeSet)
    {
        throw std::runtime_error("node set expected");
    }
    XPathNodeSet* rightNodeSet = static_cast<XPathNodeSet*>(rightResult.get());
    int m = rightNodeSet->Length();
    for (int i = 0; i < m; ++i)
    {
        result->Add((*rightNodeSet)[i]);
    }
    return std::unique_ptr<XPathObject>(result.release());
}

XPathCombineStepExpr::XPathCombineStepExpr(XPathExpr* left_, XPathExpr* right_) : XPathBinaryExpr(left_, right_)
{
}

std::unique_ptr<XPathObject> XPathCombineStepExpr::Evaluate(XPathContext& context)
{
    std::unique_ptr<XPathNodeSet> result(new XPathNodeSet());
    std::unique_ptr<XPathObject> leftResult = Left()->Evaluate(context);
    if (leftResult->Type() != XPathObjectType::nodeSet)
    {
        throw std::runtime_error("node set expected");
    }
    XPathNodeSet* leftNodeSet = static_cast<XPathNodeSet*>(leftResult.get());
    int n = leftNodeSet->Length();
    for (int i = 0; i < n; ++i)
    {
        xml::Node* node = (*leftNodeSet)[i];
        XPathContext rightContext(node, i + 1, n);
        std::unique_ptr<XPathObject> rightResult = Right()->Evaluate(rightContext);
        if (rightResult->Type() != XPathObjectType::nodeSet)
        {
            throw std::runtime_error("node set expected");
        }
        XPathNodeSet* rightNodeSet = static_cast<XPathNodeSet*>(rightResult.get());
        int m = rightNodeSet->Length();
        for (int i = 0; i < m; ++i)
        {
            xml::Node* node = (*rightNodeSet)[i];
            result->Add(node);
        }
    }
    return std::unique_ptr<XPathObject>(result.release());
}

XPathRootNodeExpr::XPathRootNodeExpr()
{
}

std::unique_ptr<XPathObject> XPathRootNodeExpr::Evaluate(XPathContext& context)
{
    std::unique_ptr<XPathNodeSet> nodeSet(new XPathNodeSet());
    if (context.Node()->Kind() == xml::NodeKind::documentNode)
    {
        nodeSet->Add(context.Node());
    }
    else
    {
        nodeSet->Add(context.Node()->OwnerDocument());
    }
    return std::unique_ptr<XPathObject>(nodeSet.release());
}

XPathFilterExpr::XPathFilterExpr(XPathExpr* expr_, XPathExpr* predicate_) : XPathUnaryExpr(expr_), predicate(predicate_)
{
}

std::unique_ptr<XPathObject> XPathFilterExpr::Evaluate(XPathContext& context)
{
    std::unique_ptr<XPathObject> result = Operand()->Evaluate(context);
    if (result->Type() != XPathObjectType::nodeSet)
    {
        throw std::runtime_error("node-set expected");
    }
    std::unique_ptr<XPathNodeSet> nodeSet(static_cast<XPathNodeSet*>(result.release()));
    std::unique_ptr<XPathNodeSet> filteredNodeSet(new XPathNodeSet());
    int n = nodeSet->Length();
    for (int i = 0; i < n; ++i)
    {
        xml::Node* node = (*nodeSet)[i];
        XPathContext context(node, i + 1, n);
        std::unique_ptr<XPathObject> result = predicate->Evaluate(context);
        bool booleanResult = false;
        if (result->Type() == XPathObjectType::number)
        {
            XPathNumber* number = static_cast<XPathNumber*>(result.get());
            if (number->Value() == context.Position())
            {
                booleanResult = true;
            }
        }
        else
        {
            XPathFunction* boolean = GetXPathLibraryFunction("boolean");
            std::vector<XPathObject*> args;
            args.push_back(result.get());
            std::unique_ptr<XPathObject> resultAsBoolean = boolean->Evaluate(context, args);
            booleanResult = static_cast<XPathBoolean*>(resultAsBoolean.get())->Value();
        }
        if (booleanResult)
        {
            filteredNodeSet->Add(node);
        }
    }
    std::swap(nodeSet, filteredNodeSet);
    return std::unique_ptr<XPathObject>(nodeSet.release());
}

class NodeSelectionOp : public xml::NodeOperation
{
public:
    NodeSelectionOp(XPathNodeTestExpr* nodeTest_, XPathNodeSet& nodeSet_, xml::Axis axis_);
    void Apply(xml::Node* node) override;
private:
    XPathNodeTestExpr* nodeTest;
    XPathNodeSet& nodeSet;
    xml::Axis axis;
};

NodeSelectionOp::NodeSelectionOp(XPathNodeTestExpr* nodeTest_, XPathNodeSet& nodeSet_, xml::Axis axis_) : nodeTest(nodeTest_), nodeSet(nodeSet_), axis(axis_)
{
}

void NodeSelectionOp::Apply(xml::Node* node)
{
    if (nodeTest->Select(node, axis))
    {
        nodeSet.Add(node);
    }
}

XPathLocationStepExpr::XPathLocationStepExpr(xml::Axis axis_, XPathNodeTestExpr* nodeTest_) : axis(axis_), nodeTest(nodeTest_)
{
}

void XPathLocationStepExpr::AddPredicate(XPathExpr* predicate)
{
    predicates.push_back(std::unique_ptr<XPathExpr>(predicate));
}

std::unique_ptr<XPathObject> XPathLocationStepExpr::Evaluate(XPathContext& context)
{
    std::unique_ptr<XPathNodeSet> nodeSet(new XPathNodeSet());
    NodeSelectionOp selectNodes(nodeTest.get(), *nodeSet, axis);
    context.Node()->Walk(selectNodes, axis);
    for (const std::unique_ptr<XPathExpr>& predicate : predicates)
    {
        std::unique_ptr<XPathNodeSet> filteredNodeSet(new XPathNodeSet());
        int n = nodeSet->Length();
        for (int i = 0; i < n; ++i)
        {
            xml::Node* node = (*nodeSet)[i];
            XPathContext context(node, i + 1, n);
            std::unique_ptr<XPathObject> result = predicate->Evaluate(context);
            bool booleanResult = false;
            if (result->Type() == XPathObjectType::number)
            {
                XPathNumber* number = static_cast<XPathNumber*>(result.get());
                if (number->Value() == context.Position())
                {
                    booleanResult = true;
                }
            }
            else
            {
                XPathFunction* boolean = GetXPathLibraryFunction("boolean");
                std::vector<XPathObject*> args;
                args.push_back(result.get());
                std::unique_ptr<XPathObject> resultAsBoolean = boolean->Evaluate(context, args);
                booleanResult = static_cast<XPathBoolean*>(resultAsBoolean.get())->Value();
            }
            if (booleanResult)
            {
                filteredNodeSet->Add(node);
            }
        }
        std::swap(nodeSet, filteredNodeSet);
    }
    return std::unique_ptr<XPathObject>(nodeSet.release());
}

class AxisMap
{
public:
    AxisMap();
    xml::Axis GetAxis(const std::string& axis) const;
private:
    std::map<std::string, xml::Axis> axisMap;
};

AxisMap::AxisMap()
{
    axisMap["ancestor"] = xml::Axis::ancestor;
    axisMap["ancestor-or-self"] = xml::Axis::ancestorOrSelf;
    axisMap["attribute"] = xml::Axis::attribute;
    axisMap["child"] = xml::Axis::child;
    axisMap["descendant"] = xml::Axis::descendant;
    axisMap["descendant-or-self"] = xml::Axis::descendantOrSelf;
    axisMap["following"] = xml::Axis::following;
    axisMap["following-sibling"] = xml::Axis::followingSibling;
    axisMap["namespace"] = xml::Axis::ns;
    axisMap["parent"] = xml::Axis::parent;
    axisMap["preceding"] = xml::Axis::preceding;
    axisMap["preceding-sibling"] = xml::Axis::precedingSibling;
    axisMap["self"] = xml::Axis::self;
}

xml::Axis AxisMap::GetAxis(const std::string& axis) const
{
    std::map<std::string, xml::Axis>::const_iterator it = axisMap.find(axis);
    if (it != axisMap.cend())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("axis '" + axis + "' not found");
    }
}

AxisMap axisMap;

xml::Axis GetAxis(const std::string& axisName)
{
    return axisMap.GetAxis(axisName);
}

XPathPILiteralTest::XPathPILiteralTest(XPathExpr* literal_) : literal(literal_)
{
}

bool XPathPILiteralTest::Select(xml::Node* node, xml::Axis axis) const
{
    if (node->Kind() == xml::NodeKind::processingInstructionNode)
    {
        xml::ProcessingInstruction* pi = static_cast<xml::ProcessingInstruction*>(node);
        if (pi->Target() == literal->TextValue())
        {
            return true;
        }
    }
    return false;
}

bool XPathCommentNodeTest::Select(xml::Node* node, xml::Axis axis) const
{
    return node->Kind() == xml::NodeKind::commentNode;
}

bool XPathTextNodeTest::Select(xml::Node* node, xml::Axis axis) const
{
    return node->Kind() == xml::NodeKind::textNode;
}

bool XPathPINodeTest::Select(xml::Node* node, xml::Axis axis) const
{
    return node->Kind() == xml::NodeKind::processingInstructionNode;
}

bool XPathPrincipalNodeTest::Select(xml::Node* node, xml::Axis axis) const
{
    if (axis == xml::Axis::attribute)
    {
        return node->Kind() == xml::NodeKind::attributeNode;
    }
    else if (axis == xml::Axis::ns)
    {
        return false; // todo
    }
    else
    {
        return node->Kind() == xml::NodeKind::elementNode;
    }
}

bool XPathAnyNodeTest::Select(xml::Node* node, xml::Axis axis) const
{
    return true;
}

XPathPrefixTest::XPathPrefixTest(const std::string& name_) : name(name_)
{
}

bool XPathPrefixTest::Select(xml::Node* node, xml::Axis axis) const
{
    if (axis == xml::Axis::attribute)
    {
        if (node->Kind() == xml::NodeKind::attributeNode)
        {
            xml::AttributeNode* attr = static_cast<xml::AttributeNode*>(node);
            if (attr->Prefix() == name)
            {
                return true;
            }
        }
    }
    else if (axis == xml::Axis::ns)
    {
        // todo
        return false;
    }
    else if (node->Kind() == xml::NodeKind::elementNode)
    {
        xml::Element* element = static_cast<xml::Element*>(node);
        if (element->Prefix() == name)
        {
            return true;
        }
    }
    return false;
}

XPathNameTest::XPathNameTest(const std::string& name_) : name(name_)
{
}

bool XPathNameTest::Select(xml::Node* node, xml::Axis axis) const
{
    if (axis == xml::Axis::attribute)
    {
        if (node->Kind() == xml::NodeKind::attributeNode)
        {
            xml::AttributeNode* attr = static_cast<xml::AttributeNode*>(node);
            if (attr->Name() == name)
            {
                return true;
            }
        }
    }
    else if (axis != xml::Axis::ns)
    {
        if (node->Kind() == xml::NodeKind::elementNode)
        {
            xml::Element* element = static_cast<xml::Element*>(node);
            if (element->Name() == name)
            {
                return true;
            }
        }
    }
    return false;
}

XPathVariableReference::XPathVariableReference(const std::string& name_) : name(name_)
{
}

XPathLiteral::XPathLiteral(const std::string& value_) : value(value_)
{
}

std::unique_ptr<XPathObject> XPathLiteral::Evaluate(XPathContext& context)
{
    return std::unique_ptr<XPathObject>(new XPathString(value));
}

XPathNumberExpr::XPathNumberExpr(const std::string& value_)
{
    std::string s = value_;
    std::stringstream strm;
    strm.str(s);
    strm >> value;
}

XPathNumberExpr::XPathNumberExpr(double value_) : value(value_)
{
}

std::unique_ptr<XPathObject> XPathNumberExpr::Evaluate(XPathContext& context)
{
    return std::unique_ptr<XPathObject>(new XPathNumber(value));
}

XPathFunctionCall::XPathFunctionCall(const std::string& functionName_) : functionName(functionName_)
{
}

void XPathFunctionCall::AddArgument(XPathExpr* argument)
{
    arguments.push_back(std::unique_ptr<XPathExpr>(argument));
}

std::unique_ptr<XPathObject> XPathFunctionCall::Evaluate(XPathContext& context)
{
    XPathFunction* function = GetXPathLibraryFunction(functionName);
    if (arguments.size() < function->MinArity() || arguments.size() > function->MaxArity())
    {
        throw std::runtime_error("function '" + functionName + "' takes " + std::to_string(function->MinArity()) + "..." + std::to_string(function->MaxArity()) + " arguments (" +
            std::to_string(arguments.size()) + " arguments provided)");
    }
    std::vector<std::unique_ptr<XPathObject>> ownedArgs;
    std::vector<XPathObject*> args;
    for (const std::unique_ptr<XPathExpr>& argument : arguments)
    {
        std::unique_ptr<XPathObject> arg = argument->Evaluate(context);
        args.push_back(arg.get());
        ownedArgs.push_back(std::move(arg));
    }
    std::unique_ptr<XPathObject> result = function->Evaluate(context, args);
    return result;
}

} // namespace xpath
