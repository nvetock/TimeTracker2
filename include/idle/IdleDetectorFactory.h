#pragma once

#include <memory>
#include "IIdleDetector.h"

namespace timetracker
{
    std::unique_ptr<IIdleDetector> createIdleDetector();
} // timetracker