#pragma once

#include <QObject>
#include "infra/AppSettingsRepository.h"
#include "infra/SessionLogRepository.h"

namespace timetracker
{
    class AppController;
    class SessionManager;
    class WorkSession;
}

namespace ui
{
    class TimeTrackerPanel;
    class MainMenuPage;
    class TrackWorkSetupPage;
    class TrackWorkTimerPage;
    class LogHistoryPage;
    class SettingsPage;

    class UiFlowController : public QObject
    {
        Q_OBJECT

    public:
        UiFlowController(TimeTrackerPanel* panel,
                         timetracker::AppController* appCtrl,
                         timetracker::SessionManager* sMgr,
                         QObject* parent = nullptr);

        void start(); // show main menu

    private:
        // Navigation helpers
        void showMainMenu();
        void showTrackWorkSetup();
        void showTrackWorkTimer(const QString& date,
                                const QString& task,
                                const QString& project,
                                const QString& description);
        void showLogHistory();
        void showSettings();

    private:
        TimeTrackerPanel*            mPanel;
        timetracker::AppController*  mAppController;
        timetracker::SessionManager* mSessionManager;
        infra::AppSettingsRepository mSettingsRepo;
        infra::AppSettings           mSettings;
        infra::SessionLogRepository* mLogRepo;
    };
}
