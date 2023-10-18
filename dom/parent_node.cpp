// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module dom.parent_node;

import dom.document;
import dom.document_fragment;
import dom.error;
import dom.visitor;
import dom.node_operation;

namespace xml {

ParentNode::ParentNode(NodeKind kind_, const lexer::SourcePos& sourcePos_, const std::string& name_) : Node(kind_, sourcePos_, name_), firstChild(nullptr), lastChild(nullptr)
{
}

ParentNode::~ParentNode()
{
    Node* child = firstChild;
    while (child)
    {
        Node* toDel = child;
        child = child->Next();
        delete toDel;
    }
}

void ParentNode::AppendChild(Node* child)
{
    if (!child)
    {
        throw XmlException("could not append child node: given child is null", GetSourcePos());
    }
    if (OwnerDocument())
    {
        OwnerDocument()->InvalidateIndex();
    }
    if (child->Parent())
    {
        child = child->Parent()->RemoveChild(child).release();
    }
    if (child->IsDocumentFragmentNode())
    {
        DocumentFragment* documentFragment = static_cast<DocumentFragment*>(child);
        Node* docFragChild = documentFragment->FirstChild();
        Node* nodeInserted = nullptr;
        while (docFragChild)
        {
            std::unique_ptr<Node> docFragChildRemoved = documentFragment->RemoveChild(docFragChild);
            AppendChild(docFragChildRemoved.release());
            docFragChild = documentFragment->FirstChild();
        }
    }
    else
    {
        if (lastChild)
        {
            lastChild->LinkAfter(child);
        }
        if (!firstChild)
        {
            firstChild = child;
        }
        child->SetOwnerDocument(OwnerDocument());
        child->SetParent(this);
        lastChild = child;
    }
}

void ParentNode::InsertBefore(Node* newChild, Node* refChild)
{
    if (!newChild)
    {
        throw XmlException("could not insert child node: given child is null", GetSourcePos());
    }
    if (refChild == nullptr)
    {
        AppendChild(newChild);
    }
    if (OwnerDocument())
    {
        OwnerDocument()->InvalidateIndex();
    }
    if (newChild->IsDocumentFragmentNode())
    {
        DocumentFragment* documentFragment = static_cast<DocumentFragment*>(newChild);
        Node* docFragChild = documentFragment->FirstChild();
        Node* nodeInserted = nullptr;
        while (docFragChild)
        {
            std::unique_ptr<Node> docFragChildRemoved = documentFragment->RemoveChild(docFragChild);
            InsertBefore(docFragChildRemoved.release(), refChild);
            docFragChild = documentFragment->FirstChild();
        }
    }
    else
    {
        if (newChild->Parent())
        {
            newChild = newChild->Parent()->RemoveChild(newChild).release();
        }
        if (firstChild == refChild)
        {
            firstChild = newChild;
        }
        newChild->SetParent(this);
        newChild->SetOwnerDocument(OwnerDocument());
        refChild->LinkBefore(newChild);
    }
}

std::unique_ptr<Node> ParentNode::RemoveChild(Node* child)
{
    if (!child)
    {
        throw XmlException("could not remove node: given child is null", GetSourcePos());
    }
    if (child->Parent() != this)
    {
        std::string errorMessage = "could not remove node: given child is not child of node '" + Name() + "'";
        lexer::SourcePos sourcePos = GetSourcePos();
        throw XmlException(errorMessage, sourcePos);
    }
    if (OwnerDocument())
    {
        OwnerDocument()->InvalidateIndex();
    }
    child->Unlink();
    if (child == firstChild)
    {
        firstChild = child->Next();
    }
    if (child == lastChild)
    {
        lastChild = child->Prev();
    }
    std::unique_ptr<Node> removedNode = std::unique_ptr<Node>(child);
    removedNode->SetOwnerDocument(nullptr);
    removedNode->SetParent(nullptr);
    return removedNode;
}

std::unique_ptr<Node> ParentNode::ReplaceChild(Node* newChild, Node* oldChild)
{
    if (!newChild)
    {
        throw XmlException("could not replace node: given child is null", GetSourcePos());
    }
    if (!oldChild)
    {
        throw XmlException("could not replace node: given old child is null", GetSourcePos());
    }
    if (oldChild->Parent() != this)
    {
        std::string errorMessage = "could not replace node: given old child is not child of node '" + Name() + "'";
        lexer::SourcePos sourcePos = GetSourcePos();
        throw XmlException(errorMessage, sourcePos);
    }
    if (OwnerDocument())
    {
        OwnerDocument()->InvalidateIndex();
    }
    if (newChild->Parent())
    {
        newChild = newChild->Parent()->RemoveChild(newChild).release();
    }
    InsertBefore(newChild, oldChild);
    return RemoveChild(oldChild);
}

void ParentNode::Accept(Visitor& visitor)
{
    Node* child = firstChild;
    while (child)
    {
        child->Accept(visitor);
        child = child->Next();
    }
}

void ParentNode::Write(util::CodeFormatter& formatter)
{
    Node* child = firstChild;
    while (child)
    {
        child->Write(formatter);
        child = child->Next();
    }
}

void ParentNode::WalkChildren(NodeOperation& operation)
{
    Node* child = firstChild;
    while (child)
    {
        operation.Apply(child);
        child = child->Next();
    }
}

void ParentNode::WalkDescendant(NodeOperation& operation)
{
    Node* child = firstChild;
    while (child)
    {
        child->WalkDescendantOrSelf(operation);
        child = child->Next();
    }
}

void ParentNode::WalkDescendantOrSelf(NodeOperation& operation)
{
    Node::WalkDescendantOrSelf(operation);
    Node* child = firstChild;
    while (child)
    {
        child->WalkDescendantOrSelf(operation);
        child = child->Next();
    }
}

void ParentNode::WalkPreceding(NodeOperation& operation)
{
    Node* preceding = Prev();
    if (preceding)
    {
        preceding->WalkPrecedingOrSelf(operation);
    }
}

void ParentNode::WalkPrecedingOrSelf(NodeOperation& operation)
{
    Node::WalkPrecedingOrSelf(operation);
    Node* child = lastChild;
    while (child)
    {
        child->WalkPrecedingOrSelf(operation);
        child = child->Prev();
    }
}

} // namespace xml
