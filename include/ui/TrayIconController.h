#pragma once

#include <QObject>
#include <QSystemTrayIcon>

class QSystemTrayIcon;
class QMenu;

namespace timetracker
{
    class WorkSession;
}

namespace ui
{
    class TrayIconController : public QObject
    {
        Q_OBJECT

    public:
        explicit TrayIconController(QWidget* parentWindow);
        ~TrayIconController() override;

        void show();

    public slots:
        void onSessionStarted(const timetracker::WorkSession& session);
        void onSessionStopped(const timetracker::WorkSession& session);

    signals:
        void openWindowRequested();
        void stopRequested();
        void quitRequested();

    private slots:
        void handleActivated(QSystemTrayIcon::ActivationReason reason);
        void handleOpen();
        void handleStop();
        void handleQuit();

    private:
        void buildTrayIcon();

    private:
        QWidget* mParentWindow;
        QSystemTrayIcon* mTrayIcon;
        QMenu* mMenu;
    };
}