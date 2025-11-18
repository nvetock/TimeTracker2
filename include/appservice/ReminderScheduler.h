#pragma once

#include <QObject>
#include <QTimer>

#include "domain/WorkSession.h"

namespace reminder
{
    enum rState : quint8 { ReminderOne, ReminderTwo, HardStop, RSTATE_COUNT };
}

namespace timetracker
{
    class ReminderScheduler : public QObject
    {
        Q_OBJECT

    public:


        /**
         * The ReminderSchedule sends nudges to an inactive user before the WorkSession is ultimately terminated.
         * @param reminderTwoThreshold (seconds) the delay between first reminder at idle start & next reminder.
         * @param hardStopThreshold (seconds) the delay between second reminder & WorkSession hard stop
         * @param parent Parent of this QObject
         */
        ReminderScheduler(int reminderTwoThreshold,
                          int hardStopThreshold,
                          QObject* parent = nullptr);

    public slots:
        void onSessionStarted();
        void onSessionStopped();

        void onUserIdle();
        void onUserNotIdle();

    signals:
        void reminderOneShown();
        void reminderTwoShown();
        void hardStopRaised();

    private slots:
        void handleTimeout();

    private:
        void reminderOne();
        void reminderTwo();
        void hardStop();

        void resetReminderState();
        void cancelTimer();

    private:
        bool mSessionActive;
        bool mUserIdle;

        int mReminderTwoThresholdSec;
        int mHardStopThresholdSec;

        reminder::rState mNextReminderToShow;

        QTimer mTimer;
    };
} // timetracker