// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>
#undef MessageBox

module wing.message_box;

import wing.control;
import util;

namespace wing {

MessageBoxResult MessageBox::Show(const std::string& message, const std::string& caption, Control* owner, int messageBoxType)
{
    std::u16string msg = util::ToUtf16(message);
    std::u16string cap = util::ToUtf16(caption);
    return static_cast<MessageBoxResult>(MessageBoxW(owner->Handle(), reinterpret_cast<LPCWSTR>(msg.c_str()), reinterpret_cast<LPCWSTR>(cap.c_str()), messageBoxType));
}

} // wing
