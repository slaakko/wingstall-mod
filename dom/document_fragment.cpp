// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module dom.document_fragment;

namespace xml {

DocumentFragment::DocumentFragment(const lexer::SourcePos& sourcePos_) : ParentNode(NodeKind::documentFragmentNode, sourcePos_, "document_fragment")
{
}

DocumentFragment* MakeDocumentFragment()
{
    return new DocumentFragment(lexer::SourcePos());
}

} // namespace xml
