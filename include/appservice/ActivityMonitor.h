#pragma once

#include <QObject>
#include <QTimer>

#include "idle/IIdleDetector.h"

namespace timetracker
{
    /**
     * The ActivityMonitor watches signals cast
     * from the OS through the IIdleDetector interface.
     *
     * On idle the ActivityMonitor will relay that signal
     * for other service apps to handle idle and re-active states accordingly.
     */
    class ActivityMonitor : public QObject
    {
        Q_OBJECT

    public:
        /**
         * @param idleDetector The OS detector (auto-gen by Factory class).
         * @param idleThresh (seconds) Threshold of inactivity that delegates idle status.
         * @param pollInterval (milliseconds) Interval of polling for inactivity.
         * @param parent parent of this QObject
         */
        ActivityMonitor(std::unique_ptr<IIdleDetector> idleDetector,
                        int idleThresh,
                        int pollInterval,
                        QObject* parent = nullptr);

    public slots:
        void startMonitoring();
        void stopMonitoring();

    signals:
        void userIsIdle();
        void userNoLongerIdle();

    private slots:
        void checkIdleStatus();

    private:
        std::unique_ptr<IIdleDetector> mIdleDetector;
        int mIdleThresholdSec;  // The threshold of when a user is declared idle.
        int mPollIntervalMs;    // The rate to receive activity input from OS.
        bool mIsIdle;           // User's activity status.

        QTimer mIdleTimer;
    };
} // timetracker