#include "appservice/AppController.h"

#include <QDebug>

#include "appservice/ActivityMonitor.h"
#include "appservice/ReminderScheduler.h"
#include "appservice/SessionManager.h"

namespace timetracker
{
    AppController::AppController(ActivityMonitor* aMtr,
                                 ReminderScheduler* rSched,
                                 SessionManager* sMgr,
                                 QObject* parent)
        : QObject{parent}
        , mActivityMtr{aMtr}
        , mReminderSched{rSched}
        , mSessionMgr{sMgr}
    { }

    bool AppController::isValid() const
    {
        if (!mSessionMgr || !mActivityMtr || !mReminderSched)
        {
            qCritical() << "[AppController] initialize()\n"
                        << "  | nullptr detected. Active?\n"
                        << "  | ActivityMonitor:" << (bool)(mActivityMtr != nullptr)
                        << "  | ReminderScheduler:" << (bool)(mReminderSched != nullptr)
                        << "  | SessionManager:" << (bool)(mSessionMgr != nullptr);
            return false;
        }
        return true;
    }

    void AppController::initialize()
    {
        if (!isValid()) return;

        /**
         * SessionManager <--> ActivityMonitor
         */

        // SessionManager signals
        QObject::connect(mSessionMgr, &SessionManager::sessionStarted,
                         mActivityMtr, &ActivityMonitor::startMonitoring);

        QObject::connect(mSessionMgr, &SessionManager::sessionStopped,
                         mActivityMtr, &ActivityMonitor::stopMonitoring);

        // ActivityMonitor signals
        QObject::connect(mActivityMtr, &ActivityMonitor::userIsIdle,
                         mSessionMgr, &SessionManager::onUserIdle);

        QObject::connect(mActivityMtr, &ActivityMonitor::userNoLongerIdle,
                         mSessionMgr, &SessionManager::onUserNotIdle);


        /**
         * SessionManager <--> ReminderScheduler
         */

        // SessionManager signals
        QObject::connect(mSessionMgr, &SessionManager::sessionStarted,
                         mReminderSched, &ReminderScheduler::onSessionStarted);

        QObject::connect(mSessionMgr, &SessionManager::sessionStopped,
                         mReminderSched, &ReminderScheduler::onSessionStopped);


        /**
         * ActivityMonitor <--> ReminderScheduler
         */

        // ActivityMonitor signals
        QObject::connect(mActivityMtr, &ActivityMonitor::userIsIdle,
                         mReminderSched, &ReminderScheduler::onUserIdle);

        QObject::connect(mActivityMtr, &ActivityMonitor::userNoLongerIdle,
                         mReminderSched, &ReminderScheduler::onUserNotIdle);


        /**
         * ReminderScheduler <--> AppController
         */

        // ReminderScheduler signals
        QObject::connect(mReminderSched, &ReminderScheduler::reminderOneShown,
                  this, &AppController::handleReminderOne);

        QObject::connect(mReminderSched, &ReminderScheduler::reminderTwoShown,
                  this, &AppController::handleReminderTwo);

        QObject::connect(mReminderSched, &ReminderScheduler::hardStopRaised,
                  this, &AppController::handleHardStop);

        qDebug() << "[AppController] initialize()\n  | Success.";
    }

    void AppController::startSessionForTask(const QString& userName, const QString& projectName,
                                            const QString& taskName, const QString& taskDescript) const
    {
        if (!isValid()) return;

        qDebug() << "[AppController] Starting session...";
        mSessionMgr->startSession(userName, projectName, taskName, taskDescript);

    }

    void AppController::stopCurrentSession(const bool aborted) const
    {
        if (!isValid()) return;
        if (!mSessionMgr->hasActiveSession())
        {
            qDebug() << "[AppController] SessionManager has no session to stop.";
            return;
        }

        qDebug() << "[AppController] Stopping session. aborted:" << aborted;
        mSessionMgr->stopSession(aborted);
    }

    void AppController::pauseCurrentSession() const
    {
        if (!isValid()) return;

        qDebug() << "[AppController] Pausing session...";
        mSessionMgr->pauseSession();
    }

    void AppController::unpauseCurrentSession() const
    {
        if (!isValid()) return;

        qDebug() << "[AppController] Resuming session...";
        mSessionMgr->unpauseSession();
    }

    void AppController::userConfirmedStillWorking() const
    {
        if (!isValid()) return;

        mReminderSched->onUserNotIdle();
    }

    void AppController::handleReminderOne()
    {
        qDebug() << "[AppController] Idle Reminder One requested by" << sender();
        emit reminderOneRequested();
    }

    void AppController::handleReminderTwo()
    {
        qDebug() << "[AppController] Idle Reminder Two requested by" << sender();
        emit reminderTwoRequested();
    }

    void AppController::handleHardStop()
    {
        qDebug() << "[AppController] Hard Stop requested by" << sender();

        mSessionMgr->stopSession(false);
        emit hardStopExecuted(*mSessionMgr->getCurrentSession());

    }

    void AppController::handleAbort()
    {
        qDebug() << "[AppController] Hard Stop requested by" << sender();

        mSessionMgr->stopSession(true);
        emit abortExecuted(*mSessionMgr->getCurrentSession());

    }

    void AppController::handleSessionStarted(const WorkSession& session)
    {
        qDebug() << "[AppController] Session started for task"
                 << session.getTaskName() << "at:"
                 << session.getStartTime().toString(Qt::ISODate);
    }

    void AppController::handleSessionStopped(const WorkSession& session)
    {
        qDebug() << "[AppController] Session stopped. Active seconds:"
                 << session.getActiveSeconds() << "Total seconds:"
                 << session.getTotalElapsedSeconds();
    }
} // timetracker