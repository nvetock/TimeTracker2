#include "idle/IdleDetectorFactory.h"

#include <QtGlobal>

#include "idle/MacIdleDetector.h"

#ifdef Q_OS_WIN
    #include "idle/WinIdleDetector.h"
#elif defined(Q_OS_MAC)
    #include "idle/MacIdleDetector.h"
#endif


namespace timetracker
{
    std::unique_ptr<IIdleDetector> createIdleDetector()
    {
#ifdef Q_OS_WIN
        return std::make_unique<WinIdleDetector>();
#elif defined(Q_OS_MAC)
        return std::make_unique<MacIdleDetector>();
#else
        struct NullIdleDetector : public IIdleDetector
        {
            int idleSeconds() const override { return 0; }
        };
        return std::make_unique<NullIdleDetector>();
#endif
    }
} // timetracker