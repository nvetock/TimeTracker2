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
        qDebug() << "[ReminderScheduler] onSessionStarted()\n  | Activated. Called by" << sender();

        mSessionActive = true;
        mUserIdle = false;
        resetReminderState();
        cancelTimer();
    }

    void ReminderScheduler::onSessionStopped()
    {
        if (!mSessionActive) return;
        qDebug() << "[ReminderScheduler] onSessionStopped()\n  | Deactivated.";

        mSessionActive = false;
        mUserIdle = false;
        resetReminderState();
        cancelTimer();
    }

    void ReminderScheduler::onUserIdle()
    {
        if (!mSessionActive) return;
        if (mUserIdle) return; // in case of repeated signals

        qDebug() << "[ReminderScheduler] onUserIdle()\n"
                 << "  | Displaying initial idle reminder.";

        mUserIdle = true;

        // Ensure curr reminder state & timer are reset.
        resetReminderState();
        cancelTimer();

        handleTimeout();
    }

    void ReminderScheduler::onUserNotIdle()
    {
        qDebug() << "[ReminderScheduler] onUserNotIdle()\n"
                 << "  | Resetting reminder state.";

        mUserIdle = false;
        resetReminderState();
        cancelTimer();
    }

    void ReminderScheduler::handleTimeout()
    {
        if (mUserIdle)
        {
            qDebug() << "[ReminderScheduler] handleTimeout()\n"
                     << "  | Handling timeout stage:" << mNextReminderToShow;
            switch (mNextReminderToShow)
            {
            case reminder::rState::ReminderOne:
                reminderOne();
                break;
            case reminder::rState::ReminderTwo:
                reminderTwo();
                break;
            case reminder::rState::HardStop:
                hardStop();
                break;
            default:
                qWarning() << "[ReminderScheduler] handleTimeout()\n  | executed in default space. rState:" << mNextReminderToShow;
                break;
            }

            // Increment and send back to enum
            quint8 reminderIdx = mNextReminderToShow;
            ++reminderIdx;
            reminderIdx = reminderIdx % static_cast<quint8>(reminder::rState::RSTATE_COUNT);

            mNextReminderToShow = static_cast<reminder::rState>(reminderIdx);
        }
    }

    void ReminderScheduler::reminderOne()
    {
        qDebug() << "[ReminderScheduler] reminderOne()\n"
                 << "  | User is now idle.";

        mTimer.start(mReminderTwoThresholdSec * 1000);
        emit reminderOneShown();
    }

    void ReminderScheduler::reminderTwo()
    {
        qDebug() << "[ReminderScheduler] reminderTwo()\n"
         << "  | Session will timeout soon";

        mTimer.start(mHardStopThresholdSec * 1000);
        emit reminderTwoShown();
    }

    void ReminderScheduler::hardStop()
    {
        qDebug() << "[ReminderScheduler] hardStop()\n"
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