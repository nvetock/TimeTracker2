#include "UiFlowController.h"

#include "TimeTrackerPanel.h"
#include "MainMenuPage.h"
#include "TrackWorkSetupPage.h"
#include "TrackWorkTimerPage.h"
#include "LogHistoryPage.h"
#include "SettingsPage.h"
#include "NewItemDialog.h"

#include <QCoreApplication>
#include <QDate>

namespace ui
{
    UiFlowController::UiFlowController(TimeTrackerPanel* panel,
                                       QObject* parent)
        : QObject(parent)
        , mPanel(panel)
        , mSettingsRepo(this)
        , mSettings(mSettingsRepo.load())
    {
    }

    void UiFlowController::start()
    {
        showMainMenu();
    }

    void UiFlowController::showMainMenu()
    {
        auto* page = new MainMenuPage(mPanel);

        connect(page, &MainMenuPage::trackWorkRequested,
                this, &UiFlowController::showTrackWorkSetup);

        connect(page, &MainMenuPage::logHistoryRequested,
                this, &UiFlowController::showLogHistory);

        connect(page, &MainMenuPage::exportRequested,
                this, &UiFlowController::showLogHistory); // placeholder for export

        connect(page, &MainMenuPage::settingsRequested,
                this, &UiFlowController::showSettings);

        connect(page, &MainMenuPage::quitRequested,
                this, []() { QCoreApplication::quit(); });

        mPanel->setPage(page);
        mPanel->setOpen(true);
    }

    void UiFlowController::showTrackWorkSetup()
    {
        auto* page = new TrackWorkSetupPage(mPanel);

        page->setAvailableTasks(mSettings.tasks);
        page->setAvailableProjects(mSettings.projects);

        connect(page, &TrackWorkSetupPage::backRequested,
                this, &UiFlowController::showMainMenu);

        connect(page, &TrackWorkSetupPage::readyClicked,
                this, [this](const QString& date,
                             const QString& task,
                             const QString& project,
                             const QString& desc)
        {
            // Here you could also talk to AppController to start a session
            QString t = task.trimmed();
            QString p = project.trimmed();

            if (!task.isEmpty() && !mSettings.tasks.contains(t, Qt::CaseInsensitive))
            {
                mSettings.tasks.append(t);
            }

            if (!project.isEmpty() && !mSettings.projects.contains(p, Qt::CaseInsensitive))
            {
                mSettings.projects.append(p);
            }

            if (!mSettingsRepo.save(mSettings))
            {
                qWarning() << "[UIFlowController} showTrackWorkSetup()\n"
                           << "  | Settings did not save to mSettingsRepo.";
            }


            showTrackWorkTimer(date, task, project, desc);
        });

        mPanel->setPage(page);
    }

    void UiFlowController::showTrackWorkTimer(const QString& date,
                                              const QString& task,
                                              const QString& project,
                                              const QString& description)
    {
        auto* page = new TrackWorkTimerPage(mPanel);

        page->setTitle("TRACK WORK");
        page->setTitle("RUNNING");
        page->setTimerText("00:00:00");
        page->setDateText(date);
        page->setProjectText(project);
        page->setTaskText(task);

        connect(page, &TrackWorkTimerPage::returnClicked,
                this, &UiFlowController::showTrackWorkSetup);

        // Here is where you'd hook page->startClicked() to AppController
        // and also a STOP button to stop the session, etc.

        mPanel->setPage(page);
        mPanel->setOpen(false); // slide mostly closed while running
    }

    void UiFlowController::showLogHistory()
    {
        auto* page = new LogHistoryPage(mPanel);
        connect(page, &LogHistoryPage::backRequested,
                this, &UiFlowController::showMainMenu);

        mPanel->setPage(page);
    }

    void UiFlowController::showSettings()
    {
        auto* page = new SettingsPage(mPanel);
        page->loadFromSettings(mSettings);

        connect(page, &SettingsPage::backRequested,
                this, &UiFlowController::showMainMenu);

        connect(page, &SettingsPage::settingsSaved,
                this, [this](const infra::AppSettings& updated)
        {
            // Merge: only overwrite fields changed in SettingsPage
            mSettings.userName      = updated.userName;
            if (!updated.saveDirectory.isEmpty())
                mSettings.saveDirectory = updated.saveDirectory;

            mSettingsRepo.save(mSettings);
            showMainMenu();
        });

        connect(page, &SettingsPage::resetTasksRequested,
                this, [this]()
        {
            mSettings.tasks.clear();
            mSettingsRepo.save(mSettings);
        });

        connect(page, &SettingsPage::resetProjectsRequested,
                this, [this]()
        {
            mSettings.projects.clear();
            mSettingsRepo.save(mSettings);
        });

        mPanel->setPage(page);
    }

    void UiFlowController::handleAddNewTask()
    {
        NewItemDialog dlg("Add new task",
                          "Enter a name for the new task:",
                          mPanel);
        if (dlg.exec() == QDialog::Accepted)
        {
            const QString name = dlg.text();
            if (!name.isEmpty())
            {
                if (!mSettings.tasks.contains(name))
                    mSettings.tasks.append(name);
                mSettingsRepo.save(mSettings);
            }
        }

        // After adding, show setup page again with refreshed lists
        showTrackWorkSetup();
    }

    void UiFlowController::handleAddNewProject()
    {
        NewItemDialog dlg("Add new project",
                          "Enter a name for the new project:",
                          mPanel);
        if (dlg.exec() == QDialog::Accepted)
        {
            const QString name = dlg.text();
            if (!name.isEmpty())
            {
                if (!mSettings.projects.contains(name))
                    mSettings.projects.append(name);
                mSettingsRepo.save(mSettings);
            }
        }

        showTrackWorkSetup();
    }
}
