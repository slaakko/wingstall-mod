// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module dom.node;

import std.core;
import util.code_formatter;
import lexer.source_pos;
import dom.axis;

export namespace xml {

enum class NodeKind
{
    attributeNode, documentFragmentNode, documentNode, elementNode, entityNode, entityReferenceNode, notationNode, processingInstructionNode, textNode, cdataSectionNode, commentNode
};

std::string NodeKindStr(NodeKind nodeKind);

class ParentNode;
class Document;
class Visitor;
class NodeOperation;

class Node
{
public:
    Node(NodeKind kind_, const lexer::SourcePos& sourcePos_, const std::string& name_);
    virtual ~Node();
    NodeKind Kind() const { return kind; }
    const lexer::SourcePos& GetSourcePos() const { return sourcePos; }
    const std::string& Name() const { return name; }
    const std::string& NamespaceUri() const { return namespaceUri; }
    void SetNamespaceUri(const std::string& namespaceUri_);
    bool IsAttributeNode() const { return kind == NodeKind::attributeNode; }
    bool IsDocumentFragmentNode() const { return kind == NodeKind::documentFragmentNode; }
    bool IsDocumentNode() const { return kind == NodeKind::documentNode; }
    bool IsElementNode() const { return kind == NodeKind::elementNode; }
    bool IsEntityNode() const { return kind == NodeKind::entityNode; }
    bool IsEntityReferenceNode() const { return kind == NodeKind::entityReferenceNode; }
    bool IsNotationNode() const { return kind == NodeKind::notationNode; }
    bool IsProcessingInstructionNode() const { return kind == NodeKind::processingInstructionNode; }
    bool IsTextNode() const { return kind == NodeKind::textNode; }
    bool IsCDataSectionNode() const { return kind == NodeKind::cdataSectionNode; }
    bool IsCommentNode() const { return kind == NodeKind::commentNode; }
    void Walk(NodeOperation& operation, Axis axis);
    virtual void WalkChildren(NodeOperation& operation);
    virtual void WalkDescendant(NodeOperation& operation);
    virtual void WalkDescendantOrSelf(NodeOperation& operation);
    void WalkParent(NodeOperation& operation);
    virtual void WalkFollowing(NodeOperation& operation);
    virtual void WalkPreceding(NodeOperation& operation);
    virtual void WalkPrecedingOrSelf(NodeOperation& operation);
    void WalkAncestor(NodeOperation& operation);
    void WalkAncestorOrSelf(NodeOperation& operation);
    void WalkFollowingSibling(NodeOperation& operation);
    void WalkPrecedingSibling(NodeOperation& operation);
    virtual void WalkAttribute(NodeOperation& operation);
    ParentNode* Parent() const { return parent; }
    Node* Prev() const { return prev; }
    Node* Next() const { return next; }
    Document* OwnerDocument() const { return ownerDocument; }
    void SetOwnerDocument(Document* ownerDocumnent_) { ownerDocument = ownerDocumnent_; }
    std::string Prefix() const;
    void SetPrefix(const std::string& prefix);
    std::string LocalName() const;
    virtual void Accept(Visitor& visitor) {}
    virtual bool HasChildNodes() const { return false; }
    virtual bool HasAttributes() const { return false; }
    virtual bool ValueContainsNewLine() const { return false; }
    virtual void Write(util::CodeFormatter& formatter) = 0;
private:
    friend class ParentNode;
    void SetParent(ParentNode* parent_) { parent = parent_; }
    void SetPrev(Node* prev_) { prev = prev_; }
    void SetNext(Node* next_) { next = next_; }
    void LinkBefore(Node* node);
    void LinkAfter(Node* node);
    void Unlink();
    NodeKind kind;
    lexer::SourcePos sourcePos;
    std::string name;
    std::string namespaceUri;
    ParentNode* parent;
    Node* prev;
    Node* next;
    Document* ownerDocument;
};

} // namespace xml
