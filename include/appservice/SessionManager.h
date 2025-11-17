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
        void onStartSession(const QString& userName,
                const QString& projectName, const QString& taskName,
                const QString& taskDescript="");
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
        void onStopSession(bool aborted = false);

    private:
        std::unique_ptr<WorkSession> mCurrentSession;
    };
} // timetracker
