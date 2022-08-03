#pragma once

// #include "Threading/Threads/ThreadManager.h"
#include <cassert>

#define ASSERT_SAVANNA_MAIN_THREAD \
    //TODO
    // assert(SavannaGetMainThreadId() == std::this_thread::get_id());

#define ASSERT_SAVANNA_RENDER_THREAD \
    // TODO
    // assert(SavannaGetRenderThreadId() == std::this_thread::get_id());