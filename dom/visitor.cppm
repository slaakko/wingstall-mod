// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module dom.visitor;

import std.core;

export namespace xml {

class Document;
class Element;
class Text;
class CDataSection;
class Comment;
class EntityReference;
class ProcessingInstruction;

class Visitor
{
public:
    virtual void BeginVisit(Document& document) {}
    virtual void EndVisit(Document& document) {}
    virtual void BeginVisit(Element& element) {}
    virtual void EndVisit(Element& element) {}
    virtual void Visit(Text& text) {}
    virtual void Visit(CDataSection& cdataSection) {}
    virtual void Visit(Comment& comment) {}
    virtual void Visit(EntityReference& entityReference) {}
    virtual void Visit(ProcessingInstruction& processingInstruction) {}
};

} // namespace xml
