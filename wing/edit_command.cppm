// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.edit_command;

import wing.edit_command_list;
import wing.text_view;

export namespace wing {

class InsertCharCommand : public EditCommand
{
public:
    InsertCharCommand(int lineIndex_, int columnIndex_, char32_t c_, bool removeIndent_);
    void Undo(TextView* textView) override;
    void Redo(TextView* textView) override;
private:
    int lineIndex;
    int columnIndex;
    char32_t c;
    bool removeIndent;
};

class DeleteCharCommand : public EditCommand
{
public:
    DeleteCharCommand(int lineIndex_, int columnIndex_, char32_t c_);
    void Undo(TextView* textView) override;
    void Redo(TextView* textView) override;
private:
    int lineIndex;
    int columnIndex;
    char32_t c;
};

class NewLineCommand : public EditCommand
{
public:
    NewLineCommand(int lineIndex_, int columnIndex_, int indent_, int numSpaces_);
    void Undo(TextView* textView) override;
    void Redo(TextView* textView) override;
private:
    int lineIndex;
    int columnIndex;
    int indent;
    int numSpaces;
};

class TabCommand : public EditCommand
{
public:
    TabCommand(int lineIndex_, int columnIndex_);
    void Undo(TextView* textView) override;
    void Redo(TextView* textView) override;
private:
    int lineIndex;
    int columnIndex;
};

class BacktabCommand : public EditCommand
{
public:
    BacktabCommand(int lineIndex_, int columnIndex_, int numSpaces_);
    void Undo(TextView* textView) override;
    void Redo(TextView* textView) override;
private:
    int lineIndex;
    int columnIndex;
    int numSpaces;
};

class IndentSelectionCommand : public EditCommand
{
public:
    IndentSelectionCommand(const Selection& selection_);
    void Undo(TextView* textView) override;
    void Redo(TextView* textView) override;
private:
    Selection selection;
};

class UnindentSelectionCommand : public EditCommand
{
public:
    UnindentSelectionCommand(const Selection& selection_);
    void Undo(TextView* textView) override;
    void Redo(TextView* textView) override;
private:
    Selection selection;
};

class InsertLinesCommand : public EditCommand
{
public:
    InsertLinesCommand(int lineIndex_, int columnIndex_, const std::vector<std::u32string>& lines_);
    void Undo(TextView* textView) override;
    void Redo(TextView* textView) override;
private:
    int lineIndex;
    int columnIndex;
    std::vector<std::u32string> lines;
};

class InsertIntoLineCommand : public EditCommand
{
public:
    InsertIntoLineCommand(int lineIndex_, int columnIndex_, const std::u32string& text_);
    void Undo(TextView* textView) override;
    void Redo(TextView* textView) override;
private:
    int lineIndex;
    int columnIndex;
    std::u32string text;
};

class RemoveSelectionCommand : public EditCommand
{
public:
    RemoveSelectionCommand(const Selection& selection_, const SelectionData& selectionData_, bool wholeLine_);
    void Undo(TextView* textView) override;
    void Redo(TextView* textView) override;
private:
    Selection selection;
    SelectionData selectionData;
    bool wholeLine;
};

} // wing

