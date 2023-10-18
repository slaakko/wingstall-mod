// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module util.init_done;

import util.ansi;
import util.unicode;
import util.socket;
import util.time;

namespace util {

void Init()
{
    UnicodeInit();
    InitSocket();
    AnsiInit();
    TimeInit();
}

void Done()
{
    TimeDone();
    AnsiDone();
    DoneSocket();
    UnicodeDone();
}

} // namespace util
