#pragma once

#include "IIdleDetector.h"

namespace timetracker
{
    class MacIdleDetector : public IIdleDetector
    {
        int idleSeconds() const override;
    };
}