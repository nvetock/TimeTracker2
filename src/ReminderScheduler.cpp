#include "appservice/ReminderScheduler.h"

#include <QDebug>

namespace timetracker
{
    ReminderScheduler::ReminderScheduler(int reminderTwoThreshold,
                                         int hardStopThreshold,
                                         QObject* parent)
        : QObject{parent}
        , mSessionActive{false}
        , mUserIdle{false}
        , mReminderTwoThresholdSec{reminderTwoThreshold}
        , mHardStopThresholdSec{hardStopThreshold}
        , mNextReminderToShow{reminder::ReminderOne}
    {
        mTimer.setSingleShot(true);

        connect(&mTimer, &QTimer::timeout,
            this, &ReminderScheduler::handleTimeout);
    }

    void ReminderScheduler::onSessionStarted()
    {
        qDebug() << "[ReminderScheduler] onSessionStarted()";

        mSessionActive = true;
        mUserIdle = true;
        resetReminderState();
        cancelTimer();
    }

    void ReminderScheduler::onSessionStopped()
    {
        if (!mSessionActive) return;
        qDebug() << "[ReminderScheduler] onSessionStopped()";

        mSessionActive = false;
        mUserIdle = false;
        resetReminderState();
        cancelTimer();
    }

    void ReminderScheduler::onUserIdle()
    {
        if (!mSessionActive) return;
        if (mUserIdle) return; // in case of repeated signals

        qDebug() << "[ReminderScheduler] onSessionStarted()\n"
                 << "  | Displaying initial idle reminder.";

        mUserIdle = true;
        resetReminderState();

        handleTimeout();
    }

    void ReminderScheduler::onUserNotIdle()
    {
        qDebug() << "[ReminderScheduler] onSessionStarted()\n"
                 << "  | ";

        mUserIdle = false;
        resetReminderState();
        cancelTimer();
    }

    void ReminderScheduler::handleTimeout()
    {
        qDebug() << "[ReminderScheduler] handleTimeout()\n"
                 << "  | Handling timeout stage:" << mNextReminderToShow;
        switch (mNextReminderToShow)
        {
        case reminder::ReminderOne:
            reminderOne();
            break;
        case reminder::ReminderTwo:
            reminderTwo();
            break;
        case reminder::HardStop:
            hardStop();
            break;
        default:
            qWarning() << "[ReminderScheduler] handleTimeout()\n  | executed in default space. rState:" << mNextReminderToShow;
            break;
        }

        // Increment and send back to enum
        quint8 reminderIdx = mNextReminderToShow;
        ++reminderIdx;
        reminderIdx = reminderIdx % reminder::rState::RSTATE_COUNT;

        mNextReminderToShow = static_cast<reminder::rState>(reminderIdx);
    }

    void ReminderScheduler::reminderOne()
    {
        qDebug() << "[ReminderScheduler] reminderOne()"
                 << "  | User is now idle.";

        mTimer.start(mReminderTwoThresholdSec);
        emit reminderOneShown();
    }

    void ReminderScheduler::reminderTwo()
    {
        qDebug() << "[ReminderScheduler] reminderTwo()"
         << "  | Session will timeout soon";

        mTimer.start(mHardStopThresholdSec);
        emit reminderTwoShown();
    }

    void ReminderScheduler::hardStop()
    {
        qDebug() << "[ReminderScheduler] hardStop()"
         << "  | Ending session...";

        // reset and cancel everything.
        resetReminderState();
        cancelTimer();

        // send signal for session shutdown.
        emit hardStopRaised();
        
    }

    void ReminderScheduler::resetReminderState()
    {
        mNextReminderToShow = reminder::ReminderOne;
    }

    void ReminderScheduler::cancelTimer()
    {
        mTimer.stop();
    }
} // timetracker