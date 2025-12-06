#pragma once

#include "BaseCardPage.h"
#include "infra/AppSettingsRepository.h"

class QLineEdit;
class QPushButton;

namespace ui
{
    class SettingsPage : public BaseCardPage
    {
        Q_OBJECT

    public:
        explicit SettingsPage(QWidget* parent = nullptr);

        /// Bind current settings into the UI fields.
        void loadFromSettings(const infra::AppSettings& settings);

    signals:
        /// User clicked SAVE: send out the updated settings.
        void settingsSaved(const infra::AppSettings& settings);

        /// User wants to clear / reset lists.
        void resetTasksRequested();
        void resetProjectsRequested();

    private slots:
        void handleBrowse();
        void handleSave();
        void handleResetTasks();
        void handleResetProjects();

    private:
        QLineEdit*   mNameEdit{nullptr};
        QLineEdit*   mSaveDirEdit{nullptr};
        QPushButton* mBrowseBtn{nullptr};
        QPushButton* mSaveBtn{nullptr};
        QPushButton* mResetTasksBtn{nullptr};
        QPushButton* mResetProjectsBtn{nullptr};
    };
}