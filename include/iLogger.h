#pragma once

#include "bulk.h"

struct ILogger {
    virtual ~ILogger() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
};