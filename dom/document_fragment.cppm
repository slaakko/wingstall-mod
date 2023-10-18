// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module dom.document_fragment;

import std.core;
import dom.parent_node;

export namespace xml {

class DocumentFragment : public ParentNode
{
public:
    DocumentFragment(const lexer::SourcePos& sourcePos_);
};

DocumentFragment* MakeDocumentFragment();

} // namespace xml
