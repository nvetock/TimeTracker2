#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <QThread>

#include "domain/WorkSession.h"
#include "include/idle/IdleDetectorFactory.h"
#include "appservice/SessionManager.h"


int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    // Inactivity tester
    auto idleDetector = timetracker::createIdleDetector();

    constexpr int inactThreshSec{5};
    constexpr int intrusiveSec{5};
    constexpr int hardStopSec{10};



    // QTimer::singleShot(0, [appController]()
    // {
    //     qDebug() << "[main] Starting test session...";
    //     appController->startSessionForTask("task-123", "test task",
    //         "project-567", "username test",
    //         "Testing AppController connections.");
    // });

    return app.exec();
}