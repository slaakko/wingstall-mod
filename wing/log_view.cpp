// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing.log_view;

import util;

namespace wing {

TextViewCreateParams& MakeTextViewCreateParams(TextViewCreateParams& createParams)
{
    createParams.controlCreateParams.WindowClassName("wing.LogView");
    createParams.FontSize(9.0f);
    return createParams;
}

LogView::LogView(TextViewCreateParams& createParams) : TextView(MakeTextViewCreateParams(createParams))
{
    SetReadOnly();
}

void LogView::WriteLine(const std::string& text)
{
    int startLineNumber = static_cast<int>(Lines().size() - 1);
    std::vector<std::u32string> lines = SplitTextIntoLines(util::ToUtf32(text));
    for (const std::u32string& line : lines)
    {
        AddLine(line);
    }
    SetCaretLineCol(static_cast<int>(Lines().size()), 1);
    SetTextExtent();
    ScrollToCaret();
    SetChanged();
    InvalidateLines(startLineNumber, static_cast<int>(startLineNumber + lines.size()));
}

} // wing
