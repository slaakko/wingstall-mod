// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.rule;

import package_editor.node;
import dom;
import rex;
import wing;
import std.core;

export namespace package_editor {

class Rule;

class Rules : public Node
{
public:
    Rules();
    void AddElements(xml::Element* directoryElement);
    void AddRule(Rule* rule) override;
    std::string ImageName() const override { return "rules.bitmap"; }
    std::string ViewName() const override { return "rules_view"; }
    wing::TreeViewNode* ToTreeViewNode(wing::TreeView* view);
    wing::Control* CreateView(wing::ImageList* imageList) override;
    int Count() const override;
    int IndexOf(const Node* child) const override;
    Node* GetNode(int index) const override;
    std::unique_ptr<Node> RemoveChild(int index) override;
    void InsertBefore(int index, std::unique_ptr<Node>&& child);
    void InsertAfter(int index, std::unique_ptr<Node>&& child);
    bool CanAdd() const override { return true; }
    void AddNew(NodeKind kind) override;
    void AddAddNewMenuItems(wing::ContextMenu* contextMenu, std::vector<std::unique_ptr<wing::ClickAction>>& clickActions) override;
    bool CanClear() const override;
    void Clear() override;
    int RuleCount() const override;
    Rule* GetRule(int index) const override;
    Rule* GetRule(const std::string& name) const override;
private:
    std::vector<std::unique_ptr<Rule>> rules;
};

enum class RuleKind
{
    exclude, include
};

enum class PathKind
{
    dir, file
};

class Rule : public Node
{
public:
    Rule();
    Rule(const std::string& namePattern_, RuleKind ruleKind_, PathKind pathKind_);
    Rule(const std::string& packageXMLFilePath, xml::Element* element);
    std::string ViewName() const override { return "rule_view"; }
    xml::Element* ToXml() const;
    std::string Text() const;
    wing::TreeViewNode* ToTreeViewNode(wing::TreeView* view);
    wing::Control* CreateView(wing::ImageList* imageList) override;
    RuleKind GetRuleKind() const { return ruleKind; }
    void SetRuleKind(RuleKind ruleKind_) { ruleKind = ruleKind_; }
    PathKind GetPathKind() const { return pathKind; }
    void SetPathKind(PathKind pathKind_) { pathKind = pathKind_; }
    bool Cascade() const { return cascade; }
    void SetCascade(bool cascade_) { cascade = cascade_; }
    void AddRule(Rule* rule) override;
    void RemoveRules();
    std::string ImageName() const override;
    std::string CascadeStr() const;
    const std::string& Value() const { return value; }
    void SetValue(const std::string& value_);
    void SetData(wing::ListViewItem* item, wing::ImageList* imageList) override;
    int Count() const override;
    int IndexOf(const Node* child) const override;
    Node* GetNode(int index) const override;
    std::unique_ptr<Node> RemoveChild(int index) override;
    void InsertBefore(int index, std::unique_ptr<Node>&& child);
    void InsertAfter(int index, std::unique_ptr<Node>&& child);
    bool Leaf() const override;
    bool CanAdd() const override;
    void AddNew(NodeKind kind) override;
    void AddAddNewMenuItems(wing::ContextMenu* contextMenu, std::vector<std::unique_ptr<wing::ClickAction>>& clickActions) override;
    bool CanEdit() const override { return true; }
    void Edit() override;
    void ExecuteDefaultAction() override { Edit(); }
    int RuleCount() const override;
    Rule* GetRule(int index) const override;
    Rule* GetRule(const std::string& name) const override;
    bool Matches(const std::u32string& name);
private:
    RuleKind ruleKind;
    PathKind pathKind;
    bool cascade;
    std::vector<std::unique_ptr<Rule>> rules;
    std::string value;
    bool compiled;
    rex::Nfa nfa;
};

Rule* MakeExcludeDebugDirRule();
Rule* MakeExcludeReleaseDirRule();
Rule* MakeExcludeX64DirRule();
Rule* MakeExcludeVSDirRule();

} // package_editor
