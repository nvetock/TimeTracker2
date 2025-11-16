#pragma once

#include "IIdleDetector.h"

namespace timetracker
{
    class WinIdleDetector : public IIdleDetector
    {
    public:
        int idleSeconds() const override;
    };
}