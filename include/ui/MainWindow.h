#pragma once

#include <QMainWindow>

class QLineEdit;
class QPushButton;
class QLabel;

namespace timetracker
{
        class AppController;
        class SessionManager;
        class WorkSession;
}


namespace ui
{
    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        MainWindow(timetracker::AppController* controller,
                   timetracker::SessionManager* sessionManager,
                   QWidget* parent = nullptr);

    public slots:
        void showReminderOneDialog();
        void showReminderTwoDialog();

        // react to session changes here
        void onSessionStarted(const timetracker::WorkSession& session);
        void onSessionStopped(const timetracker::WorkSession& session);

    private slots:
        void handleStartClicked();
        void handleStopClicked();
        //ToDo: Add pausing functionality?

    private:
        void buildUi();
        void updateStatusLabel(const QString& text) const;

    private:
        timetracker::AppController* mController;
        timetracker::SessionManager* mSessionManager;

        QLineEdit* mTaskIdEdit;
        QLineEdit* mProjectIdEdit;
        QLineEdit* mUserIdEdit;
        QLineEdit* mDescriptionEdit;
        QPushButton* mStartButton;
        QPushButton* mStopButton;
        QLabel* mStatusLabel;
    };
}
