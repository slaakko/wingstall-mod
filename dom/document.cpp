// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module dom.document;

import dom.visitor;
import dom.error;
import dom.index;

namespace xml {

Document::Document() :
    ParentNode(NodeKind::documentNode, lexer::SourcePos(), "document"), documentElement(nullptr), indexValid(false), xmlStandalone(false)
{
}

Document::Document(const lexer::SourcePos& sourcePos_) :
    ParentNode(NodeKind::documentNode, sourcePos_, "document"), documentElement(nullptr), indexValid(false), xmlStandalone(false)
{
}

Element* Document::GetElementById(const std::string& elementId)
{
    if (!indexValid)
    {
        index.clear();
        BuildIndex(this);
        indexValid = true;
    }
    auto it = index.find(elementId);
    if (it != index.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void Document::AppendChild(Node* child)
{
    if (!child)
    {
        throw XmlException("could not append node: given child is null", GetSourcePos());
    }
    CheckValidInsert(child);
    ParentNode::AppendChild(child);
    if (child->IsElementNode())
    {
        documentElement = static_cast<Element*>(child);
    }
}

void Document::InsertBefore(Node* newChild, Node* refChild)
{
    if (!newChild)
    {
        throw XmlException("could not insert node: given child is null", GetSourcePos());
    }
    CheckValidInsert(newChild);
    ParentNode::InsertBefore(newChild, refChild);
    if (newChild->IsElementNode())
    {
        documentElement = static_cast<Element*>(newChild);
    }
}

std::unique_ptr<Node> Document::RemoveChild(Node* child)
{
    if (!child)
    {
        throw XmlException("could not remove node: given child is null", GetSourcePos());
    }
    std::unique_ptr<Node> removedChild = ParentNode::RemoveChild(child);
    if (removedChild.get() == documentElement)
    {
        documentElement = nullptr;
    }
    return removedChild;
}

std::unique_ptr<Node> Document::ReplaceChild(Node* newChild, Node* oldChild)
{
    if (!newChild)
    {
        throw XmlException("could not replace node: given child is null", GetSourcePos());
    }
    if (!oldChild)
    {
        throw XmlException("could not replace node: given old child is null", GetSourcePos());
    }
    CheckValidInsert(newChild);
    if (newChild->IsElementNode())
    {
        std::unique_ptr<Node> removed = RemoveChild(oldChild);
        AppendChild(newChild);
        return removed;
    }
    else
    {
        return ParentNode::ReplaceChild(newChild, oldChild);
    }
}

void Document::Accept(Visitor& visitor)
{
    visitor.BeginVisit(*this);
    ParentNode::Accept(visitor);
    visitor.EndVisit(*this);
}

void Document::Write(util::CodeFormatter& formatter)
{
    if (!xmlVersion.empty() && !xmlEncoding.empty())
    {
        formatter.WriteLine("<?xml version=\"" + xmlVersion + "\" encoding=\"" + xmlEncoding + "\"?>");
    }
    ParentNode::Write(formatter);
}

void Document::InvalidateIndex()
{
    indexValid = false;
}

void Document::CheckValidInsert(Node* node)
{
    if (node->IsElementNode())
    {
        if (documentElement)
        {
            std::string errorMessage = "attempted to insert an element to a document that already has a document element";
            lexer::SourcePos sourcePos = GetSourcePos();
            throw XmlException(errorMessage, sourcePos);
        }
    }
}

std::unique_ptr<Document> MakeDocument()
{
    return std::unique_ptr<Document>(new Document(lexer::SourcePos()));
}

} // namespace xml
