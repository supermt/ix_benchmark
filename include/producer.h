//
// Created by jinghuan on 12/30/20.
//

#ifndef IX_BENCHMARK_PRODUCER_H
#define IX_BENCHMARK_PRODUCER_H
#pragma once
#include "rate_limiter.h"
#include <pthread.h>
#include "generator/key_gen.h"

namespace IX_NAME_SPACE{
    // default time window with duration of one second
    auto default_time_window_size = IX_NAME_SPACE::ms_clock::duration(1000);



};



#endif //IX_BENCHMARK_PRODUCER_H
