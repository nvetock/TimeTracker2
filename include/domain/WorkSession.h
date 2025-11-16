#pragma once
#include <qdatetime.h>
#include <string>

namespace timetracker
{
    class WorkSession
    {
    public:
        enum class Status { NotStarted, Running, Paused, Completed, Aborted };

        WorkSession();
        ~WorkSession() = default;
        void start(const QString& userName,
                   const QString& projectName,
                   const QString& taskName,
                   const QString& taskDescription = "",
                   const QDateTime& startTime = QDateTime::currentDateTimeUtc());

        void stop(const QDateTime& endTime = QDateTime::currentDateTimeUtc(),
                  bool aborted = false);

        qint64 totalElapsedSeconds() const;
        qint64 activeSeconds() const;
        void addInactivity(qint64 seconds);

    private:
        static QString generateSessionId();

    private:
        QString mSessionId;
        QString mProjectName;
        QString mTaskName;
        QString mUserName;
        QString mTaskDesc;
        QDateTime mStartTime;
        QDateTime mEndTime;
        qint64 mInactiveSeconds;
        Status mStatus;

    public:
        //region GETTERS

        [[nodiscard]] const QString& getSessionId() const
        {
            return mSessionId;
        }

        [[nodiscard]] const QString& getTakeName() const
        {
            return mTaskName;
        }

        [[nodiscard]] const QString& getProjectName() const
        {
            return mProjectName;
        }

        [[nodiscard]] const QString& getUserName() const
        {
            return mUserName;
        }

        [[nodiscard]] const QString& getTaskDescription() const
        {
            return mTaskDesc;
        }

        [[nodiscard]] const QDateTime& getStartTime() const
        {
            return mStartTime;
        }

        [[nodiscard]] const QDateTime& getEndTime() const
        {
            return mEndTime;
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
