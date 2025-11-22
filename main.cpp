#include <QApplication>
#include <QDebug>
#include <iostream>
#include <QTimer>
#include <QThread>
#include <QFontDatabase>
#include <QFile>

#include "domain/WorkSession.h"
#include "idle/IdleDetectorFactory.h"
#include "appservice/ActivityMonitor.h"
#include "appservice/ReminderScheduler.h"
#include "appservice/SessionManager.h"
#include "appservice/AppController.h"
#include "src/ui/TimeTrackerPanel.h"
#include "src/ui/TrackWorkSetupPage.h"
#include "src/ui/TrackWorkTimerPage.h"
#include "src/ui/UiFlowController.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

// FONTS
    int f1 = QFontDatabase::addApplicationFont(":/fonts/resources/fonts/BarlowCondensed-SemiBold.ttf");
    int f2 = QFontDatabase::addApplicationFont(":/fonts/resources/fonts/BarlowSemiCondensed-Bold.ttf");
    int f3 = QFontDatabase::addApplicationFont(":/fonts/resources/fonts/Barlow-Medium.ttf");
    int f4 = QFontDatabase::addApplicationFont(":/fonts/resources/fonts/Barlow-SemiBold.ttf");

    if (QFile qssFile(":/styles/resources/styles/app.qss"); qssFile.open(QFile::ReadOnly | QFile::Text))
    {
        const QString style = QString::fromUtf8(qssFile.readAll());
        app.setStyleSheet(style);
    }
    else
    {
        qWarning() << "[main] Could not read app.qss";
    }

    if (f1 == -1 || f2 == -1 || f3 == -1 || f4 == -1)
    {
        qWarning() << "[main] Font not found.\n"
                   << "  | f1:" << f1
                   << "\n   | f2:" << f2
                   << "\n   | f3:" << f3
                   << "\n   | f4:" << f4;
    }

    constexpr int inactiveThresh{10};
    constexpr int reminderPollIntervalMS{1000};
    constexpr int warningThresh{10};
    constexpr int hardStopThresh{5};

    // Inactivity tester
    auto idleDetector = timetracker::createIdleDetector();

    auto* aMtr = new timetracker::ActivityMonitor(
        std::move(idleDetector),
        inactiveThresh,
        reminderPollIntervalMS,
        &app);

    auto* rSched = new timetracker::ReminderScheduler(
        warningThresh,
        hardStopThresh,
        &app);

    auto* sMgr = new timetracker::SessionManager(&app);

    auto* appController = new timetracker::AppController(aMtr, rSched, sMgr);
    appController->initialize();


    auto* panel = new ui::TimeTrackerPanel{};
    auto* flow = new ui::UiFlowController(panel, &app);

    panel->show();
    flow->start();

    //auto* trackPage = new ui::TrackWorkSetupPage{};
    //panel->setPage(trackPage);


// CONNECTIONS
    QObject::connect(appController, &timetracker::AppController::reminderOneRequested,
        []()
        {
            qDebug() << "[main] ReminderOne requested.";
        });

    QObject::connect(appController, &timetracker::AppController::reminderTwoRequested,
        []()
        {
            qDebug() << "[main] ReminderTwo requested.";
        });

    QObject::connect(appController, &timetracker::AppController::hardStopExecuted,
        []()
        {
            qDebug() << "[main] Hard stop requested.";
            QCoreApplication::quit();
        });

    appController->startSessionForTask(
        "userNNName", "project123", "task2222", "description here..."
    );

//Timer State
    QObject::connect(sMgr, &timetracker::SessionManager::sessionStarted,
        panel, [panel](const timetracker::WorkSession&)
        {
            panel->setOpen(false);
        });

    QObject::connect(sMgr, &timetracker::SessionManager::sessionStopped,
        panel, [panel](const timetracker::WorkSession&)
        {
            panel->setOpen(true);
        });

    // QTimer::singleShot(0, [sMgr]()
    // {
    //     qDebug() << "[main] Starting test...";
    //
    //     sMgr->onStartSession("namison", "proj", "t123", "A description here.");
    //
    // });

    // QTimer::singleShot(0, [appController]()
    // {
    //     qDebug() << "[main] Starting test session...";
    //     appController->startSessionForTask("task-123", "test task",
    //         "project-567", "username test",
    //         "Testing AppController connections.");
    // });

    return app.exec();
}
