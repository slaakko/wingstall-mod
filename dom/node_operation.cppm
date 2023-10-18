// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module dom.node_operation;

import std.core;
import dom.node;

export namespace xml {

class NodeOperation
{
public:
    virtual ~NodeOperation();
    virtual void Apply(Node* node) = 0;
};

} // namespace xml
