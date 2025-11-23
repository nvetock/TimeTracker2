#include "appservice/SessionManager.h"

namespace timetracker
{
    SessionManager::SessionManager(QObject* parent)
        : QObject{parent}
        , mCurrentSession{nullptr}
    {
    }

    bool SessionManager::isValid(const std::string& funcName) const
    {
        if (mCurrentSession == nullptr)
        {
            qWarning() << "[SessionManager]" << funcName << "\n  | Current WorkSession* is nullptr";
            return false;
        }
        return true;
    }

    bool SessionManager::hasActiveSession() const
    {
        if (!isValid("hasActiveSession()")) return false;

        return mCurrentSession->getStatus() == WorkSession::Status::Running;
    }

    const WorkSession* SessionManager::getCurrentSession() const
    {
        if (!isValid("getCurrentSession()")) return nullptr;
        return mCurrentSession.get();
    }

    void SessionManager::addInactivityToCurrentSession(const qint64 seconds) const
    {
        if (!isValid("addInactivityToCurrentSession()")) return;
        mCurrentSession->addInactivity(seconds);
    }

    void SessionManager::startSession(const QString& userName,
                const QString& projectName, const QString& taskName,
                const QString& taskDescript)
    {
        // If a session is already running auto-stop and start new one
        // ToDo: Is there a better way to handle / will this abort session?
        if (hasActiveSession()) stopSession(false);

        mCurrentSession = std::make_unique<WorkSession>();
        mCurrentSession->start(userName, projectName, taskName, taskDescript);

        // Only emit if this process was a success
        if (mCurrentSession)
        {
            emit sessionStarted(*mCurrentSession);
            emit sessionUpdated(*mCurrentSession);
            emit sessionStatusChanged(*mCurrentSession);
        }
        else
        {
            qWarning() << "[SessionManager] startSession()\n  | Unable to start process for task:" << taskName;
        }
    }

    void SessionManager::stopSession(bool aborted)
    {
        if (!isValid("stopSession()")) return;

        mCurrentSession->stop();
        emit sessionStopped(*mCurrentSession);
        emit sessionUpdated(*mCurrentSession); // stopping updates WorkSession
        emit sessionStatusChanged(*mCurrentSession);
    }

    void SessionManager::pauseSession()
    {
        if (!isValid("pauseSession()")) return;

        mCurrentSession->pause();
        emit sessionPaused(*mCurrentSession);
        emit sessionStatusChanged(*mCurrentSession);

    }

    void SessionManager::unpauseSession()
    {
        if (!isValid("unpauseSession()")) return;

        mCurrentSession->unpause();

        emit sessionResumed(*mCurrentSession);
        emit sessionUpdated(*mCurrentSession);
        emit sessionStatusChanged(*mCurrentSession);
    }

    void SessionManager::onUserIdle()
    {
        if (!isValid("onUserIdle()")) return;
        pauseSession();
    }

    void SessionManager::onUserNotIdle()
    {
        if (!isValid("onUserNotIdle()")) return;
        unpauseSession();
    }
}
