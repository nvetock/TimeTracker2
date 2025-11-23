#include "UiFlowController.h"

#include "TimeTrackerPanel.h"
#include "MainMenuPage.h"
#include "TrackWorkSetupPage.h"
#include "TrackWorkTimerPage.h"
#include "LogHistoryPage.h"
#include "SettingsPage.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDate>

#include "appservice/AppController.h"
#include "appservice/SessionManager.h"

namespace ui
{
    UiFlowController::UiFlowController(TimeTrackerPanel* panel,
                                       timetracker::AppController* appCtrl,
                                       timetracker::SessionManager* sMgr,
                                       QObject* parent)
        : QObject{parent}
        , mPanel{panel}
        , mAppController{appCtrl}
        , mSessionManager{sMgr}
        , mSettingsRepo{this}
        , mSettings{mSettingsRepo.load()}
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


            showTrackWorkTimer(date, t, p, desc);
        });

        mPanel->setPage(page);
    }

    void UiFlowController::showTrackWorkTimer(const QString& date,
                                              const QString& task,
                                              const QString& project,
                                              const QString& description)
    {
        auto* page = new TrackWorkTimerPage(mPanel);

        page->setTitle("NOT STARTED");
        page->setTimerText("00:00:00");
        page->setDateText(date);
        page->setProjectText(project);
        page->setTaskText(task);
        page->setDescriptionText(description);

        connect(page, &TrackWorkTimerPage::returnClicked,
                this, &UiFlowController::showTrackWorkSetup);

        connect(page, &TrackWorkTimerPage::startClicked,
            this, [this, task, project, description]()
            {
                if (!mAppController) return;

                // Grab username from settings
                const QString userName = mSettings.userName.isEmpty()
                ? QStringLiteral("User")
                : mSettings.userName;

                mAppController->startSessionForTask( userName, project, task, description);
            });

        connect(page, &TrackWorkTimerPage::pauseClicked,
            this, [this, page]()
            {
                if (!mAppController) return;

                if (page->isPaused())
                    mAppController->unpauseCurrentSession();
                    // ToDo: Change button text here. & below
                else
                {
                    mAppController->pauseCurrentSession();
                }
            });

        connect(page, &TrackWorkTimerPage::stopClicked,
            this, [this]()
            {
                if (!mAppController) return;

                mAppController->stopCurrentSession();
            });

        // Connect Session Status' to UI text
        if (mSessionManager)
        {
            connect(mSessionManager, &timetracker::SessionManager::sessionStatusChanged,
                    page, [page](const timetracker::WorkSession& session)
                    {
                        if (!page) return;

                        switch (session.getStatus())
                        {
                        case timetracker::WorkSession::Status::Running:
                            page->setTitle("RUNNING");
                            page->setRecordingActive(true);
                            break;
                        case timetracker::WorkSession::Status::Paused:
                            page->setTitle("PAUSED");
                            page->setRecordingActive(false);
                            break;
                        case timetracker::WorkSession::Status::Completed:
                            page->setTitle("COMPLETED");
                            page->setRecordingActive(false);
                            break;
                        case timetracker::WorkSession::Status::Timeout:
                            page->setTitle("TIMEOUT");
                            page->setRecordingActive(false);
                            break;
                        default:
                            break;
                        }
                    });
        }

        /*
        // Connect hard stop to UI text
        if (mAppController)
        {
            connect(mAppController, &timetracker::AppController::hardStopExecuted,
                page, [page](const timetracker::WorkSession& session)
            {
                if (session.getStatus() == timetracker::WorkSession::Status::Timeout)
                {
                    page->setTitle("TIMEOUT");
                    page->setRecordingActive(false);
                }
            });
        }
        */

        // Here is where you'd hook page->startClicked() to AppController
        // and also a STOP button to stop the session, etc.

        mPanel->setPage(page);
        mPanel->setOpen(true); // slide mostly closed while running
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

}
