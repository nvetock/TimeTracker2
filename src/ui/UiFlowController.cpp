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
#include <QDir>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <algorithm>

#include "Export2.h"
#include "appservice/AppController.h"
#include "appservice/SessionManager.h"
#include "infra/SessionLogRepository.h"

namespace exporter
{
    QVector<infra::SessionLogEntry> filterLogs(const QVector<infra::SessionLogEntry>& all,
                                               const QDate& fromDate, const QDate& toDate)
    {
        if (!fromDate.isValid() || !toDate.isValid() || fromDate > toDate) return all;

        QVector<infra::SessionLogEntry> out;
        out.reserve(all.size());

        for (const auto& e : all)
        {
            const QDate d = e.date;

            if (!d.isValid()) continue;
            if (d < fromDate || d > toDate) continue;

            out.push_back(e);
        }

        // Order by date then startTime
        std::sort(out.begin(), out.end(),
            [](const infra::SessionLogEntry& a, const infra::SessionLogEntry& b)
            {
                const QDate da = a.date;
                const QDate db = b.date;
                if (da != db) return da < db;
                return a.startTime < b.startTime;
            });

        return out;
    }

    bool exportLogsToCsv(const QVector<infra::SessionLogEntry>& logs,
                         const QString& filePath)
    {
        QFile f(filePath);
        if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        {
            qWarning() << "[Export] Failed to open CSV file for write:" << filePath;
            return false;
        }

        QTextStream out(&f);
        out.setEncoding(QStringConverter::Utf8);

        out << "userName,projectName,taskName,description,status,"
            << "date,startTime,endTime,activeSeconds,totalSeconds\n";

        auto csvEscape = [](const QString& s) -> QString
        {
            QString v = s;
            v.replace('"', "\"\"");
            return '"' + v + '"';
        };

        for (const auto& e : logs)
        {
            out << csvEscape(e.userName)      << ','
                << csvEscape(e.projectName)   << ','
                << csvEscape(e.taskName)      << ','
                << csvEscape(e.description)   << ','
                << csvEscape(e.status)        << ','
                << csvEscape(e.date.toString())          << ','
                << csvEscape(e.startTime.toString())     << ','
                << csvEscape(e.endTime.toString())       << ','
                << e.activeSeconds            << ','
                << e.totalSeconds             << '\n';
        }
        return true;
    }

    bool exportLogsToJson(const QVector<infra::SessionLogEntry>& logs,
                          const QString& filePath)
    {
        QJsonArray arr;

        for (const auto& e : logs)
        {
            QJsonObject obj;
            obj["userName"]      = e.userName;
            obj["projectName"]   = e.projectName;
            obj["taskName"]      = e.taskName;
            obj["description"]   = e.description;
            obj["status"]        = e.status;
            obj["date"]          = e.date.toString();
            obj["startTime"]     = e.startTime.toString();
            obj["endTime"]       = e.endTime.toString();
            obj["activeSeconds"] = e.activeSeconds;
            obj["totalSeconds"]  = e.totalSeconds;
            arr.append(obj);
        }

        QJsonObject root;
        root["version"] = 1;
        root["entries"] = arr;

        QFile f(filePath);
        if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            qWarning() << "[Export] Failed to open JSON file for write:" << filePath;
            return false;
        }

        QJsonDocument doc(root);
        f.write(doc.toJson(QJsonDocument::Indented));
        f.close();
        return true;
    }

}


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
        QString baseDir = mSettings.saveDirectory;
        if (baseDir.isEmpty())
        {
            baseDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        }

        QDir dir{baseDir};
        const QString filePath = dir.filePath("session_log.json");

        mLogRepo = new infra::SessionLogRepository(filePath, this);

        // ToDo: Move this logic out of the UiFlowController
        if (mSessionManager && mLogRepo)
        {
            connect(mSessionManager, &timetracker::SessionManager::sessionStopped,
                    this, [this](const timetracker::WorkSession& session)
                    {
                        using Status = timetracker::WorkSession::Status;
                        const Status st = session.getStatus();

                        if (st != Status::Completed && st != Status::Timeout) return;

                        infra::SessionLogEntry e;
                        e.userName = mSettings.userName;
                        e.projectName = session.getProjectName();
                        e.taskName = session.getTaskName();
                        e.description = session.getTaskDescription();
                        e.status = (st == Status::Completed) ? QStringLiteral("Completed") : QStringLiteral("Timeout");
                        e.date = session.getCreationDate();
                        e.startTime = session.getStartTime();
                        e.endTime = session.getEndTime();
                        e.activeSeconds = session.getActiveSeconds();
                        e.totalSeconds = session.getTotalElapsedSeconds();

                        if (!mLogRepo->append(e))
                        {
                            qWarning() << "[UiFlowController] ctor()\n"
                                << "  | Failed to append session log entry.";
                        }
                    });
        }
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
                this, &UiFlowController::showExport); // placeholder for export

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

        connect(page, &BaseCardPage::menuRequested,
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

        connect(page, &TrackWorkTimerPage::menuRequested,
                this, &UiFlowController::showMainMenu);

        connect(page, &TrackWorkTimerPage::startClicked,
                this, [this, task, project, description]()
                {
                    if (!mAppController) return;

                    // Grab username from settings
                    const QString userName = mSettings.userName.isEmpty()
                                                 ? QStringLiteral("User")
                                                 : mSettings.userName;

                    mAppController->startSessionForTask(userName, project, task, description);
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

                        page->applySessionStatus(session.getStatus());
                    });
        }

        // Here is where you'd hook page->startClicked() to AppController
        // and also a STOP button to stop the session, etc.

        mPanel->setPage(page);
        mPanel->setOpen(true); // slide mostly closed while running
    }

    void UiFlowController::showLogHistory()
    {
        auto* page = new LogHistoryPage(mPanel);
        connect(page, &LogHistoryPage::menuRequested,
                this, &UiFlowController::showMainMenu);

        mPanel->setPage(page);
    }

    void UiFlowController::showSettings()
    {
        auto* page = new SettingsPage(mPanel);
        page->loadFromSettings(mSettings);

        connect(page, &BaseCardPage::menuRequested,
                this, &UiFlowController::showMainMenu);

        connect(page, &SettingsPage::settingsSaved,
                this, [this](const infra::AppSettings& updated)
                {
                    // Merge: only overwrite fields changed in SettingsPage
                    mSettings.userName = updated.userName;
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

    void UiFlowController::showExport()
    {
        auto* page = new Export2(mPanel);
        page->setTitle("EXPORT");

        connect(page, &Export2::settingsRequested,
                this, &UiFlowController::showSettings);

        connect(page, &Export2::menuRequested,
                this, &UiFlowController::showMainMenu);

        connect(page, &Export2::exportRequested,
                this, [this, page](const QString& format,
                             const QDate& from,
                             const QDate& to)
                {
                    // 1) load logs
                    const auto all = mLogRepo->loadAll();

                    QVector<infra::SessionLogEntry> filtered;

                    // "all" mode: from & to are invalid (QDate()), just use all
                    if (!from.isValid() || !to.isValid())
                    {
                        filtered = all;
                    }
                    else
                    {
                        filtered = exporter::filterLogs(all, from, to);
                    }

                    if (filtered.isEmpty())
                    {
                        qWarning() << "[UiFlowController] No logs to export for chosen range.";
                        page->showExportResult(false, "Failed to write export file.");
                        return;
                    }

                    // 2) pick save location
                    const QString defaultDir =
                        mSettings.saveDirectory.isEmpty()
                            ? QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                            : mSettings.saveDirectory;

                    QString filterStr;
                    if (format == "csv")
                        filterStr = "CSV Files (*.csv)";
                    else
                        filterStr = "JSON Files (*.json)";

                    const QString suggestedName =
                        (format == "csv")
                            ? QStringLiteral("timelog_export.csv")
                            : QStringLiteral("timelog_export.json");

                    const QString filePath =
                        QFileDialog::getSaveFileName(mPanel,
                                                     "Export time logs",
                                                     QDir(defaultDir).filePath(suggestedName),
                                                     filterStr);

                    if (filePath.isEmpty())
                        return;

                    // 3) write file
                    bool ok{false};
                    if (format == "csv")
                    {
                        ok = exporter::exportLogsToCsv(filtered, filePath);
                    }
                    else
                    {
                        ok = exporter::exportLogsToJson(filtered, filePath);
                    }

                    // 4) export result
                    ok ? page->showExportResult(true, "Export successful.")
                       : page->showExportResult(false, "Failed to write export file.");
                });

        mPanel->setPage(page);
    }
}
