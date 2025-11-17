#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <QTimer>
#include <QThread>

#include "domain/WorkSession.h"
#include "idle/IdleDetectorFactory.h"
#include "appservice/ActivityMonitor.h"
#include "appservice/ReminderScheduler.h"
#include "appservice/SessionManager.h"
#include "appservice/AppController.h"


int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

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