#include "appservice/AppController.h"

#include <QDebug>

#include "appservice/ActivityMonitor.h"
#include "appservice/ReminderScheduler.h"
#include "appservice/SessionManager.h"

namespace timetracker
{
    AppController::AppController(class ActivityMonitor aMtr, class ReminderScheduler* rSched,
        class SessionManager* sMgr, QObject* parent)
    {
    }

    void AppController::initialize()
    {
    }

    void AppController::startSessionForTask(const QString& userName, const QString& taskName,
        const QString& projectName, const QString& taskDescript)
    {
    }

    void AppController::pauseCurrentSession(bool desiredPauseState)
    {
    }

    void AppController::stopCurrentSession(bool aborted) const
    {
    }

    void AppController::userConfirmedStillWorking()
    {
    }

    void AppController::handleReminderOne()
    {
    }

    void AppController::handleReminderTwo()
    {
    }

    void AppController::handleHardStop()
    {
    }

    void AppController::handleSessionStarted(const WorkSession& session)
    {
    }

    void AppController::handleSessionStopped(const WorkSession& session)
    {
    }
} // timetracker