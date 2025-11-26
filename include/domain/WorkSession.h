#pragma once
#include <QString>
#include <QTime>
#include <QDate>
#include <string>

namespace timetracker
{
    class WorkSession
    {
    public:
        enum class Status { NotStarted, Running, Paused, Completed, Timeout };

        WorkSession();
        ~WorkSession() = default;

        void start(const QString& userName,
                   const QString& projectName,
                   const QString& taskName,
                   const QString& taskDescription = "",
                   const QTime& startTime = QTime::currentTime());

        void stop(const QTime& endTime = QTime::currentTime(),
                  bool aborted = false);

        void pause(const QTime& pauseTime = QTime::currentTime());
        void unpause(const QTime& unpauseTime = QTime::currentTime());

        qint64 getTotalElapsedSeconds() const;
        qint64 getActiveSeconds() const;
        void addInactivity(qint64 seconds);

    private:
        static QString generateSessionId();

    private:
        QString mSessionId;
        QString mProjectName;
        QString mTaskName;
        QString mUserName;
        QString mTaskDesc;
        QDate mCreationDate;
        QTime mStartTime;
        QTime mEndTime;
        QTime mPauseStartTime;
        qint64 mInactiveSeconds;
        Status mStatus;

    public:
        //region GETTERS

        [[nodiscard]] const QString& getSessionId() const
        {
            return mSessionId;
        }

        [[nodiscard]] const QString& getProjectName() const
        {
            return mProjectName;
        }

        [[nodiscard]] const QString& getTaskName() const
        {
            return mTaskName;
        }

        [[nodiscard]] const QString& getUserName() const
        {
            return mUserName;
        }

        [[nodiscard]] const QString& getTaskDescription() const
        {
            return mTaskDesc;
        }

        [[nodiscard]] const QDate& getCreationDate() const
        {
            return mCreationDate;
        }

        [[nodiscard]] const QTime& getStartTime() const
        {
            return mStartTime;
        }

        [[nodiscard]] const QTime& getEndTime() const
        {
            return mEndTime;
        }

        [[nodiscard]] const QTime& getPauseStartTime() const
        {
            return mPauseStartTime;
        }

        [[nodiscard]] qint64 getInactiveSeconds() const
        {
            return mInactiveSeconds;
        }

        [[nodiscard]] Status getStatus() const
        {
            return mStatus;
        }
        //endregion GETTERS
    };

}
