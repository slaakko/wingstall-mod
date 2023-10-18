// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.message_box;

import std.core;

export namespace wing {

class Control;

enum class MessageBoxResult : int
{
    ok = 1,
    cancel = 2,
    abort = 3,
    retry = 4,
    ignore = 5,
    yes = 6,
    no = 7,
    tryAgain = 10,
    continue_ = 11
};

class MessageBox
{
public:
    static MessageBoxResult Show(const std::string& message, const std::string& caption, Control* owner, int messageBoxType);
};

} // wing
