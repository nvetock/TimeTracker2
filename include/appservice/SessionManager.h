#pragma once

#include <QObject>

#include "domain/WorkSession.h"

namespace timetracker
{
    class SessionManager : public QObject
    {
        Q_OBJECT

    public:
        explicit SessionManager(QObject* parent = nullptr);

        /**
         * @return True if an active session is currently running or paused.
         */
        [[nodiscard]] bool hasActiveSession() const;

        /**
         * Starts a new Session that stores information like time, task & project details into a single object.
         * @param userName user's name (from UserPref)
         * @param taskName this task's name
         * @param projectName this task's project name
         * @param taskDescript (optional) Any details the user wants to specify on the work.
         */
        void startSession(const QString& userName,
                            const QString& projectName, const QString& taskName,
                            const QString& taskDescript="");

        void stopSession(bool aborted = false);
        void pauseSession();
        void unpauseSession();

        /**
         * @return The current WorkSession object being read/written to.
         */
        [[nodiscard]] const WorkSession* getCurrentSession() const;
        // Pass the accumulated idle time over to the current WorkSession object.
        void addInactivityToCurrentSession(qint64 seconds) const;
    signals:
        void sessionStarted(const timetracker::WorkSession& session);
        void sessionPaused(const timetracker::WorkSession& session);
        void sessionResumed(const timetracker::WorkSession& session);
        void sessionStopped(const timetracker::WorkSession& session);
        void sessionUpdated(const timetracker::WorkSession& session);

    public slots:
        /**
         * When a user goes idle this slot will handle how the
         * data collection occurring in WorkSession will respond.
         */
        void onUserIdle();
        /**
         * Handles a user becoming active again, creates a small update in
         * WorkSession data object then resumes.
         */
        void onUserNotIdle();

    private:
        [[nodiscard]] bool isValid(const std::string& funcName) const;
    private:
        std::unique_ptr<WorkSession> mCurrentSession;
    };
} // timetracker
