#include "domain/WorkSession.h"

#include <QUuid>

namespace timetracker
{
    WorkSession::WorkSession()
        : mSessionId{generateSessionId()}
        , mProjectName{""}
        , mTaskName{""}
        , mUserName{""}
        , mTaskDesc{""}
        , mCreationDate{QDate::currentDate()}
        , mStartTime{QTime{}}
        , mEndTime{QTime{}}
        , mPauseStartTime{QTime{}}
        , mInactiveSeconds{0}
        , mStatus{Status::NotStarted}
    {
    }

    void WorkSession::start(const QString& userName, const QString& projectName, const QString& taskName,
        const QString& taskDescription, const QTime& startTime)
    {
        qDebug() << "[WorkSession] Session created for task:" << taskName;

        mUserName = userName;
        mProjectName = projectName;
        mTaskName = taskName;
        mTaskDesc = taskDescription;
        mStartTime = startTime;
        mCreationDate = QDate::currentDate();

        mStatus = Status::Running;
    }

    void WorkSession::stop(const QTime& endTime, const bool aborted)
    {
        qDebug() << "[WorkSession] Session stopped for task:" << mTaskName;

        mEndTime = endTime;

        mStatus = aborted
         ? Status::Timeout
         : Status::Completed;
    }

    void WorkSession::pause(const QTime& pauseTime)
    {
        if (mStatus != Status::Running)
        {
            qWarning() << "[WorkSession] Task" << mTaskName
                       << "is unable to be paused with status:" << static_cast<int>(mStatus);
            return;
        }
        mPauseStartTime = pauseTime;
        mStatus = Status::Paused;
        qDebug() << "[WorkSession] Pause activated on task:" << mTaskName;
    }

    void WorkSession::unpause(const QTime& unpauseTime)
    {
        if (mStatus != Status::Paused) return;
        qint64 pausedSeconds = mPauseStartTime.secsTo(unpauseTime);
        addInactivity(pausedSeconds);
        // unset pause time to return false on valid check
        mStatus = Status::Running;
        mPauseStartTime = QTime{};
    }

    qint64 WorkSession::getTotalElapsedSeconds() const
    {
        return mStartTime.secsTo(mEndTime);
    }

    qint64 WorkSession::getActiveSeconds() const
    {
        const qint64 totalSeconds = getTotalElapsedSeconds();
        const qint64 totalActiveSeconds = totalSeconds - mInactiveSeconds;

        qDebug() << "[WorkSession] Total elapsed seconds: " << totalSeconds
                 << "\n  | inactive seconds:" << mInactiveSeconds
                 << "\n  | total active time:" << totalActiveSeconds;

        return totalActiveSeconds;
    }

    void WorkSession::addInactivity(const qint64 seconds)
    {
        mInactiveSeconds += seconds;
    }

    QString WorkSession::generateSessionId()
    {
        return QUuid::createUuid().toString();
    }
}
