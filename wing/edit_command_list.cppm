// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.edit_command_list;

import std.core;

export namespace wing {

class TextView;
class MenuItem;

class EditCommand
{
public:
    EditCommand();
    virtual ~EditCommand();
    EditCommand(const EditCommand&) = delete;
    EditCommand(EditCommand&&) = delete;
    EditCommand& operator=(const EditCommand&) = delete;
    EditCommand& operator=(EditCommand&&) = delete;
    virtual void Undo(TextView* textView) = 0;
    virtual void Redo(TextView* textView) = 0;
};

class GroupCommand : public EditCommand
{
public:
    GroupCommand();
    void AddCommand(EditCommand* command);
    void Undo(TextView* textView) override;
    void Redo(TextView* textView) override;
private:
    std::vector<std::unique_ptr<EditCommand>> commands;
};

class EditCommandList
{
public:
    EditCommandList(TextView* textView_);
    EditCommandList(const EditCommandList&) = delete;
    EditCommandList(EditCommandList&&) = delete;
    EditCommandList& operator=(const EditCommandList&) = delete;
    EditCommandList& operator=(EditCommandList&&) = delete;
    void SetMenuItems(MenuItem* undoMenuItem_, MenuItem* redoMenuItem_);
    void UpdateMenuItems();
    bool CanUndo() const;
    bool CanRedo() const;
    void AddCommand(EditCommand* command);
    void BeginGroup();
    void EndGroup();
    void Undo();
    void Redo();
private:
    TextView* textView;
    int currentIndex;
    bool inGroup;
    MenuItem* undoMenuItem;
    MenuItem* redoMenuItem;
    std::vector<std::unique_ptr<EditCommand>> commands;
    std::unique_ptr<GroupCommand> groupCommand;
};

} // wing
