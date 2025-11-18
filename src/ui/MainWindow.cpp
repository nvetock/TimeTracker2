#include "MainWindow.h"

#include "appservice/SessionManager.h"

namespace ui
{
    MainWindow::MainWindow(timetracker::AppController* controller,
                           timetracker::SessionManager* sessionManager,
                           QWidget* parent)
        : QMainWindow{parent}
        , mController{controller}
        , mSessionManager{sessionManager}
    {
        buildUi();
        setWindowTitle("Time Tracker");

        if (mSessionManager)
        {
            connect(mSessionManager, &timetracker::SessionManager::sessionStarted,
             this, &MainWindow::onSessionStarted);

            connect(mSessionManager, &timetracker::SessionManager::sessionStopped,
             this, &MainWindow::onSessionStopped);
        }
    }

    void MainWindow::buildUi()
    {
        auto* central = new QWidget(this);
        auto* mainLayout = new QVBoxLayout(central);

        auto* formLayout = new QFormLayout();
    }

    void MainWindow::showReminderOneDialog()
    {

    }

    void MainWindow::showReminderTwoDialog()
    {
    }

    void MainWindow::onSessionStarted(const timetracker::WorkSession& session)
    {
    }

    void MainWindow::onSessionStopped(const timetracker::WorkSession& session)
    {
    }

    void MainWindow::handleStartClicked()
    {
    }

    void MainWindow::handleStopClicked()
    {
    }

    void MainWindow::updateStatusLabel(const QString& text) const
    {
    }
}
