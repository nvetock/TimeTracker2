#pragma once

#include <QObject>
#include "domain/WorkSession.h"

namespace timetracker
{
    class AppController : public QObject
    {
        Q_OBJECT

    public:
        AppController(class ActivityMonitor* aMtr,
                      class ReminderScheduler* rSched,
                      class SessionManager* sMgr,
                      QObject* parent = nullptr);

        /**
         * Initializes different service mgr objects.
         * Establishes connections so mgrs can talk to eachother.
         */
        void initialize();

    public slots:
        /**
         * Starts a new Session that stores information like time, task & project details into a single object.
         * @param userName user's name (from UserPref)
         * @param taskName this task's name
         * @param projectName this task's project name
         * @param taskDescript (optional) Any details the user wants to specify on the work.
         */
        void startSessionForTask(const QString& userName,
                                 const QString& projectName,
                                 const QString& taskName,
                                 const QString& taskDescript="") const;

        void pauseCurrentSession() const;
        void unpauseCurrentSession() const;
        void stopCurrentSession(bool aborted = false) const;

        bool getPauseStatus() const;
        /**
         * Used to confirm a user is still working.
         */
        void userConfirmedStillWorking() const;
    signals:
        void reminderOneRequested();
        void reminderTwoRequested();
        void hardStopExecuted(const WorkSession& session);
        void abortExecuted(const WorkSession& session);
    // handle different signals and how to manage them.
    private slots:
        /**
         * User's computer has passed first idle threshold and sends a small pop-up notifier.
         * Emits `reminderOneRequested` signal.
         */
        void handleReminderOne();
        /**
         * User's computer has passed second idle threshold and is preparing for hard stop.
         * Emits `reminderTwoRequested` signal.
         */
        void handleReminderTwo();
        /**
         * Timer is cancelled. This will auto-complete current session.
         * Emits `hardStopExecuted` signal.
         */
        void handleHardStop();
        void handleAbort();
        /**
         * a handler for any needs on session start-up.
         * @param session Current WorkSession object.
         */
        void handleSessionStarted(const WorkSession& session);
        /**
         * a handler for any needs on session termination.
         * @param session Current WorkSession object.
         */
        void handleSessionStopped(const WorkSession& session);


    private:
        bool isValid() const;
    private:
        class ActivityMonitor* mActivityMtr;
        class ReminderScheduler* mReminderSched;
        class SessionManager* mSessionMgr;

    };
} // timetracker