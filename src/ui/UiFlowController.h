#pragma once

#include <QObject>
#include "infra/AppSettingsRepository.h"

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

        // Handlers
        void handleAddNewTask();
        void handleAddNewProject();

    private:
        TimeTrackerPanel*           mPanel{nullptr};
        infra::AppSettingsRepository mSettingsRepo;
        infra::AppSettings           mSettings;
    };
}
