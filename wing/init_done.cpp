// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing.init_done;

import wing.metrics;
import wing.base;
import wing.application;
import wing.shell;

namespace wing {

void Init(HINSTANCE instance)
{
    WingInit(instance);
    ApplicationInit();
    LoadMetrics();
    ShellInit();
}

void Done()
{
    ShellDone();
    ApplicationDone();
    WingDone();
}

} // wing