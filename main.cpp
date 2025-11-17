#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <QTimer>
#include <QThread>

#include "domain/WorkSession.h"
#include "include/idle/IdleDetectorFactory.h"
#include "appservice/SessionManager.h"


int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    // Inactivity tester
    //auto idleDetector = timetracker::createIdleDetector();

    auto* sMgr = new timetracker::SessionManager();


    QTimer::singleShot(0, [sMgr]()
    {
        qDebug() << "[main] Starting test...";

        sMgr->onStartSession("namison", "proj", "t123", "A description here.");

    });

    // QTimer::singleShot(0, [appController]()
    // {
    //     qDebug() << "[main] Starting test session...";
    //     appController->startSessionForTask("task-123", "test task",
    //         "project-567", "username test",
    //         "Testing AppController connections.");
    // });

    return app.exec();
}