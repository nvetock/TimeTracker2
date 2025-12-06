#include "SettingsPage.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QCursor>

namespace ui
{
    SettingsPage::SettingsPage(QWidget* parent)
        : BaseCardPage(parent)
    {
        setTitle("SETTINGS");

        auto* body   = getBodyLayout();
        auto* footer = getFooterLayout();

        // --- Form area ----------------------------------------------------
        auto* formLayout = new QFormLayout();
        formLayout->setContentsMargins(0, 0, 0, 0);
        formLayout->setSpacing(8);

        // Name
        mNameEdit = new QLineEdit(this);
        mNameEdit->setObjectName("SettingsNameEdit");
        mNameEdit->setPlaceholderText("Your name (for logs)");

        // Save directory + browse
        auto* saveDirRow = new QHBoxLayout();
        saveDirRow->setContentsMargins(0, 0, 0, 0);
        saveDirRow->setSpacing(4);

        mSaveDirEdit = new QLineEdit(this);
        mSaveDirEdit->setObjectName("SettingsSaveDirEdit");
        mSaveDirEdit->setPlaceholderText("Save directory for logs/CSV");

        mBrowseBtn = new QPushButton(tr("Browse"), this);
        mBrowseBtn->setObjectName("SettingsBrowseButton");
        mBrowseBtn->setCursor(Qt::PointingHandCursor);
        mBrowseBtn->setFixedHeight(28);

        saveDirRow->addWidget(mSaveDirEdit, 1);
        saveDirRow->addWidget(mBrowseBtn, 0);

        // Reset buttons row
        auto* resetRow = new QHBoxLayout();
        resetRow->setContentsMargins(0, 0, 0, 0);
        resetRow->setSpacing(8);

        mResetTasksBtn = new QPushButton(tr("Reset tasks"), this);
        mResetTasksBtn->setObjectName("SettingsResetTasksButton");
        mResetTasksBtn->setCursor(Qt::PointingHandCursor);

        mResetProjectsBtn = new QPushButton(tr("Reset projects"), this);
        mResetProjectsBtn->setObjectName("SettingsResetProjectsButton");
        mResetProjectsBtn->setCursor(Qt::PointingHandCursor);

        resetRow->addWidget(mResetTasksBtn);
        resetRow->addWidget(mResetProjectsBtn);
        resetRow->addStretch();

        formLayout->addRow(tr("NAME"), mNameEdit);
        formLayout->addRow(tr("SAVE LOCATION"), saveDirRow);
        formLayout->addRow(tr("DATA"), resetRow);

        body->addLayout(formLayout);
        body->addStretch();

        // --- Footer: Save button + maybe Cancel/Back ----------------------
        mSaveBtn = new QPushButton(tr("SAVE"), this);
        mSaveBtn->setObjectName("PrimaryButton");
        mSaveBtn->setCursor(Qt::PointingHandCursor);
        mSaveBtn->setFixedHeight(44);
        mSaveBtn->setFixedWidth(220);

        auto* saveGroup = new QVBoxLayout();
        saveGroup->setContentsMargins(0, 0, 0, 0);
        saveGroup->setSpacing(8);

        saveGroup->addWidget(mSaveBtn, 0, Qt::AlignHCenter);

        footer->addLayout(saveGroup);

        // --- Connections --------------------------------------------------
        connect(mBrowseBtn, &QPushButton::clicked,
                this, &SettingsPage::handleBrowse);

        connect(mSaveBtn, &QPushButton::clicked,
                this, &SettingsPage::handleSave);

        connect(mResetTasksBtn, &QPushButton::clicked,
                this, &SettingsPage::handleResetTasks);

        connect(mResetProjectsBtn, &QPushButton::clicked,
                this, &SettingsPage::handleResetProjects);


        // The back chevron signal comes from BaseCardPage (backRequested)
        // and is handled by whatever controller hosts this page.
    }

    void SettingsPage::loadFromSettings(const infra::AppSettings& settings)
    {
        if (mNameEdit)
            mNameEdit->setText(settings.userName);

        if (mSaveDirEdit)
            mSaveDirEdit->setText(settings.saveDirectory);
    }

    void SettingsPage::handleBrowse()
    {
        const QString startDir =
            mSaveDirEdit && !mSaveDirEdit->text().isEmpty()
                ? mSaveDirEdit->text()
                : QString{};

        const QString dir = QFileDialog::getExistingDirectory(
            this,
            tr("Select save directory"),
            startDir
        );

        if (!dir.isEmpty() && mSaveDirEdit)
        {
            mSaveDirEdit->setText(dir);
        }
    }

    void SettingsPage::handleSave()
    {
        infra::AppSettings settings;
        settings.userName      = mNameEdit    ? mNameEdit->text().trimmed()      : QString{};
        settings.saveDirectory = mSaveDirEdit ? mSaveDirEdit->text().trimmed()   : QString{};
        // tasks/projects remain whatever the controller has in memory and will merge

        emit settingsSaved(settings);
    }

    void SettingsPage::handleResetTasks()
    {
        emit resetTasksRequested();
    }

    void SettingsPage::handleResetProjects()
    {
        emit resetProjectsRequested();
    }
}
