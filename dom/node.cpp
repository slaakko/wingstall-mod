// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module dom.node;

import dom.error;
import dom.axis;
import dom.node_operation;
import dom.parent_node;

namespace xml {

std::string NodeKindStr(NodeKind nodeKind)
{
    switch (nodeKind)
    {
    case NodeKind::attributeNode: return "attributeNode";
    case NodeKind::documentFragmentNode: return "documentFragementNode";
    case NodeKind::documentNode: return "documentNode";
    case NodeKind::elementNode: return "elementNode";
    case NodeKind::entityNode: return "entityNode";
    case NodeKind::entityReferenceNode: return "entityReferenceNode";
    case NodeKind::notationNode: return "notationNode";
    case NodeKind::processingInstructionNode: return "processingInstructionNode";
    case NodeKind::textNode: return "textNode";
    case NodeKind::cdataSectionNode: return "cdataSectionNode";
    case NodeKind::commentNode: return "commentNode";
    }
    return "<unknown node>";
}

Node::Node(NodeKind kind_, const lexer::SourcePos& sourcePos_, const std::string& name_) :
    kind(kind_), sourcePos(sourcePos_), name(name_), parent(nullptr), prev(nullptr), next(nullptr), ownerDocument(nullptr)
{
}

Node::~Node()
{
}

void Node::SetNamespaceUri(const std::string& namespaceUri_)
{
    namespaceUri = namespaceUri_;
}

std::string Node::Prefix() const
{
    if (IsElementNode() || IsAttributeNode())
    {
        auto colonPos = name.find(':');
        if (colonPos != std::string::npos)
        {
            return name.substr(0, colonPos);
        }
    }
    return std::string();
}

void Node::SetPrefix(const std::string& prefix)
{
    if (IsElementNode() || IsAttributeNode())
    {
        auto colonPos = name.find(':');
        if (prefix.empty())
        {
            if (colonPos != std::string::npos)
            {
                name = name.substr(colonPos + 1);
            }
        }
        else
        {
            if (colonPos != std::string::npos)
            {
                name = prefix + ":" + name.substr(colonPos + 1);
            }
            else
            {
                name = prefix + ":" + name;
            }
        }
    }
    else
    {
        throw XmlException("cannot set prefix for this node type", GetSourcePos());
    }
}

std::string Node::LocalName() const
{
    if (IsElementNode() || IsAttributeNode())
    {
        auto colonPos = name.find(':');
        if (colonPos != std::string::npos)
        {
            return name.substr(colonPos + 1);
        }
        else
        {
            return name;
        }
    }
    else
    {
        return std::string();
    }
}

void Node::LinkBefore(Node* node)
{
    if (prev)
    {
        prev->next = node;
    }
    node->prev = prev;
    node->next = this;
    prev = node;
}

void Node::LinkAfter(Node* node)
{
    if (next)
    {
        next->prev = node;
    }
    node->prev = this;
    node->next = next;
    next = node;
}

void Node::Unlink()
{
    if (prev)
    {
        prev->next = next;
    }
    if (next)
    {
        next->prev = prev;
    }
}

void Node::Walk(NodeOperation& operation, Axis axis)
{
    switch (axis)
    {
    case Axis::child: WalkChildren(operation); break;
    case Axis::descendant: WalkDescendant(operation); break;
    case Axis::descendantOrSelf: WalkDescendantOrSelf(operation); break;
    case Axis::parent: WalkParent(operation); break;
    case Axis::ancestor: WalkAncestor(operation); break;
    case Axis::ancestorOrSelf: WalkAncestorOrSelf(operation); break;
    case Axis::followingSibling: WalkFollowingSibling(operation); break;
    case Axis::precedingSibling: WalkPrecedingSibling(operation); break;
    case Axis::following: WalkFollowing(operation); break;
    case Axis::preceding: WalkPreceding(operation); break;
    case Axis::attribute: WalkAttribute(operation); break;
    case Axis::self: operation.Apply(this); break;
    }
}

void Node::WalkChildren(NodeOperation& operation)
{
}

void Node::WalkDescendant(NodeOperation& operation)
{
}

void Node::WalkDescendantOrSelf(NodeOperation& operation)
{
    operation.Apply(this);
}

void Node::WalkParent(NodeOperation& operation)
{
    if (parent)
    {
        operation.Apply(parent);
    }
}

void Node::WalkFollowing(NodeOperation& operation)
{
    Node* n = next;
    if (n)
    {
        n->WalkDescendantOrSelf(operation);
    }
}

void Node::WalkPreceding(NodeOperation& operation)
{
}

void Node::WalkPrecedingOrSelf(NodeOperation& operation)
{
    operation.Apply(this);
}

void Node::WalkAncestor(NodeOperation& operation)
{
    if (parent)
    {
        parent->WalkAncestorOrSelf(operation);
    }
}

void Node::WalkAncestorOrSelf(NodeOperation& operation)
{
    operation.Apply(this);
    if (parent)
    {
        parent->WalkAncestorOrSelf(operation);
    }
}

void Node::WalkFollowingSibling(NodeOperation& operation)
{
    Node* n = next;
    while (n)
    {
        operation.Apply(n);
        n = n->next;
    }

}

void Node::WalkPrecedingSibling(NodeOperation& operation)
{
    Node* p = prev;
    while (p)
    {
        operation.Apply(p);
        p = p->prev;
    }
}

void Node::WalkAttribute(NodeOperation& operation)
{
}

} // namespace xml
