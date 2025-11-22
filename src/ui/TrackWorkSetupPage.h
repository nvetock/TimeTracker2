#pragma once

#include "BaseCardPage.h"

class QDateEdit;
class QComboBox;
class QLineEdit;
class QPushButton;

namespace ui
{
    class TrackWorkSetupPage : public BaseCardPage
    {
        Q_OBJECT

    public:
        explicit TrackWorkSetupPage(QWidget* parent = nullptr);

        // Populate dropdowns from JSON data
        void setAvailableTasks(const QStringList& tasks);
        void setAvailableProjects(const QStringList& projects);

        // Read current values
        QString getSelectedTaskName() const;
        QString getSelectedProjectName() const;
        QString getDescription() const;

    signals:
        void readyClicked(const QString& taskName,
            const QString& projectName,
            const QString& description);

        // Called when user chooses "Add new task/project"
        void addNewTaskRequested();
        void addNewProjectRequested();

    private slots:
        void handleTaskChanged(int index);
        void handleProjectChanged(int index);
        void handleReady();

    private:
        void rebuildTaskCombo(const QStringList& tasks);
        void rebuildProjectCombo(const QStringList& projects);

        QLabel* mDate;
        QComboBox* mTaskCombo;
        QComboBox* mProjectCombo;
        QLineEdit* mDescriptionEdit;
        QPushButton* mReadyBtn;
        QPushButton* mBackBtn;

        // For tagging 'Add new' entries
        static constexpr int RoleKind = Qt::UserRole + 1;
    };
} // ui