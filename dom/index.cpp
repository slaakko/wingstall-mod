// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module dom.index;

import std.core;
import dom.visitor;
import dom.document;
import dom.element;

namespace xml {

class BuildIndexVisitor : public Visitor
{
public:
    BuildIndexVisitor(std::map<std::string, Element*>& index_);
    void BeginVisit(Element& element) override;
private:
    std::map<std::string, Element*>& index;
};

BuildIndexVisitor::BuildIndexVisitor(std::map<std::string, Element*>& index_) : index(index_)
{
}

void BuildIndexVisitor::BeginVisit(Element& element)
{
    std::string id = element.GetAttribute("id");
    if (!id.empty())
    {
        index[id] = &element;
    }
}

void BuildIndex(Document* document)
{
    BuildIndexVisitor visitor(document->Index());
    document->Accept(visitor);
}

} // namespace xml
