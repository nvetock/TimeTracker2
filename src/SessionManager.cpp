#include "appservice/SessionManager.h"

namespace timetracker
{
    SessionManager::SessionManager(QObject* parent)
        : QObject{parent}
        , mCurrentSession{nullptr}
    {
    }

    bool SessionManager::hasActiveSession() const
    {
        if (mCurrentSession == nullptr)
        {
            qWarning() << "[SessionManager] hasActiveSession()\n  | Current WorkSession* is nullptr";
            return false;
        }

        return mCurrentSession->getStatus() == WorkSession::Status::Running;
    }

    const WorkSession* SessionManager::getCurrentSession() const
    {
        if (mCurrentSession == nullptr)
        {
            qWarning() << "[SessionManager] getCurrentSession()\n  | Current WorkSession* is nullptr";
            return nullptr;
        }
        return mCurrentSession.get();
    }

    void SessionManager::addInactivityToCurrentSession(const qint64 seconds) const
    {
        if (mCurrentSession == nullptr)
        {
            qWarning() << "[SessionManager] addInactivityToCurrentSession()\n  | Current WorkSession* is nullptr";
            return;
        }
        mCurrentSession->addInactivity(seconds);
    }

    void SessionManager::onStartSession(const QString& userName,
                const QString& projectName, const QString& taskName,
                const QString& taskDescript)
    {
        // If a session is already running auto-stop and start new one
        // ToDo: Is there a better way to handle / will this abort session?
        if (hasActiveSession()) onStopSession(false);

        mCurrentSession = std::make_unique<WorkSession>();
        mCurrentSession->start(userName, projectName, taskName, taskDescript);

        // Only emit if this process was a success
        if (mCurrentSession)
        {
            emit sessionStarted(*mCurrentSession);
            emit sessionUpdated(*mCurrentSession);
        }
        else
        {
            qWarning() << "[SessionManager] onStartSession()\n  | Unable to start process for task:" << taskName;
        }
    }

    void SessionManager::onPauseSession()
    {
        if (mCurrentSession == nullptr)
        {
            qWarning() << "[SessionManager] onPauseSession()\n  | Current WorkSession* is nullptr";
            return;
        }

        mCurrentSession->pause();
        emit sessionPaused(*mCurrentSession);
    }

    void SessionManager::onUnpauseSession()
    {
        if (mCurrentSession == nullptr)
        {
            qWarning() << "[SessionManager] onUnpauseSession()\n  | Current WorkSession* is nullptr";
            return;
        }

        mCurrentSession->unpause();
        emit sessionResumed(*mCurrentSession);
        emit sessionUpdated(*mCurrentSession); // inactive time will have updated.
    }

    void SessionManager::onStopSession(bool aborted)
    {
        if (mCurrentSession == nullptr)
        {
            qWarning() << "[SessionManager] onStopSession()\n  | Current WorkSession* is nullptr";
            return;
        }
        mCurrentSession->stop();
        emit sessionStopped(*mCurrentSession);
        emit sessionUpdated(*mCurrentSession); // stopping updates WorkSession
    }
}
