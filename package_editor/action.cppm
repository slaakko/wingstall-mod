// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.action;

import wing;
import std.core;

export namespace package_editor {

class Node;
enum class NodeKind;

class OpenAndExpandAction : public wing::ClickAction
{
public:
    OpenAndExpandAction(wing::MenuItem* menuItem_, Node* node_);
    void Execute() override;
private:
    Node* node;
};

class OpenAction : public wing::ClickAction
{
public:
    OpenAction(wing::MenuItem* menuItem_, Node* node_);
    void Execute() override;
private:
    Node* node;
};

class AddAction : public wing::ClickAction
{
public:
    AddAction(wing::MenuItem* menuItem_, Node* node_, NodeKind kind_);
    void Execute() override;
private:
    Node* node;
    NodeKind kind;
};

class EditAction : public wing::ClickAction
{
public:
    EditAction(wing::MenuItem* menuItem_, Node* node_);
    void Execute() override;
private:
    Node* node;
};

class RemoveAction : public wing::ClickAction
{
public:
    RemoveAction(wing::MenuItem* menuItem_, Node* node_);
    void Execute() override;
private:
    Node* node;
};

class MoveUpAction : public wing::ClickAction
{
public:
    MoveUpAction(wing::MenuItem* menuItem_, Node* node_);
    void Execute() override;
private:
    Node* node;
};

class MoveDownAction : public wing::ClickAction
{
public:
    MoveDownAction(wing::MenuItem* menuItem_, Node* node_);
    void Execute() override;
private:
    Node* node;
};

class AddVSRulesAction : public wing::ClickAction
{
public:
    AddVSRulesAction(wing::MenuItem* menuItem_, Node* node_);
    void Execute() override;
private:
    Node* node;
};

class ExcludeAction : public wing::ClickAction
{
public:
    ExcludeAction(wing::MenuItem* menuItem_, Node* node_);
    void Execute() override;
private:
    Node* node;
};

class IncludeAction : public wing::ClickAction
{
public:
    IncludeAction(wing::MenuItem* menuItem_, Node* node_);
    void Execute() override;
private:
    Node* node;
};

class SaveAction : public wing::ClickAction
{
public:
    SaveAction(wing::MenuItem* menuItem_, Node* node_);
    void Execute() override;
private:
    Node* node;
};

class BuildAction : public wing::ClickAction
{
public:
    BuildAction(wing::MenuItem* menuItem_, Node* node_);
    void Execute() override;
private:
    Node* node;
};

class CloseAction : public wing::ClickAction
{
public:
    CloseAction(wing::MenuItem* menuItem_, Node* node_);
    void Execute() override;
private:
    Node* node;
};

class ClearAction : public wing::ClickAction
{
public:
    ClearAction(wing::MenuItem* menuItem_, Node* node_);
    void Execute() override;
private:
    Node* node;
};

} // package_editor
