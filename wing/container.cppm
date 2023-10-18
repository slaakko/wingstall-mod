// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.container;

import std.core;

export namespace wing {

class Component;

class Container
{
public:
    Container(Component* parent_);
    ~Container();
    bool IsEmpty() const { return firstChild == nullptr; }
    Component* Parent() const { return parent; }
    Component* FirstChild() const { return firstChild; }
    Component* LastChild() const { return lastChild; }
    void AddChild(Component* child);
    std::unique_ptr<Component> RemoveChild(Component* child);
    void InsertBefore(Component* child, Component* before);
    void InsertAfter(Component* child, Component* after);
private:
    Component* parent;
    Component* firstChild;
    Component* lastChild;
};

} // wing
