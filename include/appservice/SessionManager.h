#pragma once

#include <qdatetime.h>
#include <QObject>

#include "domain/WorkSession.h"

namespace timetracker
{
    class SessionManager : public QObject
    {
        Q_OBJECT

    public:
        explicit SessionManager(QObject* parent = nullptr);

        [[nodiscard]] bool hasActiveSession() const;
        [[nodiscard]] const WorkSession* getCurrentSession() const;
        // Pass the accumulated idle time over to the current WorkSession object.
        void addInactivityToCurrentSession(qint64 seconds) const;
    signals:
        void sessionStarted(const timetracker::WorkSession& session);
        void sessionStopped(const timetracker::WorkSession& session);
        void sessionUpdated(const timetracker::WorkSession& session);

    public slots:
        void startSession();

        void stopSession(bool aborted = false);

    private:
        std::unique_ptr<WorkSession> mCurrentSession;
    };
} // timetracker
