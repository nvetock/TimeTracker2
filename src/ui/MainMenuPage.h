#pragma once

#include "BaseCardPage.h"

class QPushButton;

namespace ui
{
    class MainMenuPage : public BaseCardPage
    {
        Q_OBJECT

    public:
        explicit MainMenuPage(QWidget* parent = nullptr);

        signals:
            void trackWorkRequested();
        void logHistoryRequested();
        void exportRequested();
        void settingsRequested();
        void quitRequested();

    private:
        QPushButton* mTrackWorkBtn{nullptr};
        QPushButton* mLogHistoryBtn{nullptr};
        QPushButton* mExportBtn{nullptr};
        QPushButton* mSettingsBtn{nullptr};
        QPushButton* mQuitBtn{nullptr};
    };
}
