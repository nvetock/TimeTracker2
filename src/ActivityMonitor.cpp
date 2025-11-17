#include "appservice/ActivityMonitor.h"

#include <QDebug>

namespace timetracker
{
    ActivityMonitor::ActivityMonitor(IIdleDetector* idleDetector,
        int idleThresh, int pollInterval, QObject* parent)
            : mIdleDetector{idleDetector}
            , mIdleThresholdSec{idleThresh}
            , mPollIntervalMs{pollInterval}
            , mIsIdle{false}
    {
        connect(&mIdleTimer, &QTimer::timeout,
            this, &ActivityMonitor::checkIdleStatus);
    }

    void ActivityMonitor::startMonitoring()
    {
        if (!mIdleDetector)
        {
            qCritical() << "[ActivityMonitor] startMonitoring()\n"
                     << "  | IdleDetector is nullptr.";
        }
        mIsIdle = false;
        mIdleTimer.start(mPollIntervalMs);
    }

    void ActivityMonitor::stopMonitoring()
    {
        mIdleTimer.stop();
        mIsIdle = false;
    }

    void ActivityMonitor::checkIdleStatus()
    {
        if (!mIdleDetector)
        {
            qCritical() << "[ActivityMonitor] startMonitoring()\n"
                     << "  | IdleDetector is nullptr.";
            return;
        }

        const int idleSeconds = mIdleDetector->idleSeconds();
        const bool shouldBeIdle = idleSeconds >= mIdleThresholdSec;

        // If the user is not currently idle, but should be
        // Send out a signal letting others know.
        if (!mIsIdle && shouldBeIdle)
        {
            mIsIdle = true;
            emit userIsIdle();
        }
        // If the user is currently idle, and should NOT be
        else if (mIsIdle && !shouldBeIdle)
        {
            mIsIdle = false;
            emit userNoLongerIdle();
        }
    }
} // timetracker