#include "SettingsPage.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QCursor>

#include "UiElemStyler.h"

namespace ui
{
    SettingsPage::SettingsPage(QWidget* parent)
        : BaseCardPage(parent)
    {
        setTitle("SETTINGS");

        auto* body   = getBodyLayout();

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
        setZeroMarginAndSpaceBetween(resetRow, 8);

        mResetTasksBtn = generateButton("Reset Tasks", "SecondaryBtn", 16, this);
        mResetProjectsBtn = generateButton("Reset Projects", "SecondaryBtn", 16, this);

        resetRow->addWidget(mResetTasksBtn);
        resetRow->addWidget(mResetProjectsBtn);

        formLayout->addRow(tr("NAME"), mNameEdit);
        formLayout->addRow(tr("SAVE LOCATION"), saveDirRow);
        formLayout->addRow(tr("DATA"), resetRow);

        body->addLayout(formLayout);
        body->addStretch();

        // --- Footer: Save button + maybe Cancel/Back ----------------------
        mSaveBtn = generateButton("Save", "PrimaryBtn", 44, this);
        mMenuBtn = generateButton("Menu", "SecondaryBtn", true, this);

        auto* btnLayout = new QVBoxLayout();
        setZeroMarginAndSpaceBetween(btnLayout, 0);
        btnLayout->addWidget(mSaveBtn, 0, Qt::AlignHCenter);
        btnLayout->addWidget(mMenuBtn, 0, Qt::AlignHCenter);

        auto* footer = getFooterLayout();
        footer->addLayout(btnLayout);
        footer->addSpacing(4);

        // --- Connections --------------------------------------------------
        connect(mBrowseBtn, &QPushButton::clicked,
                this, &SettingsPage::handleBrowse);

        connect(mSaveBtn, &QPushButton::clicked,
                this, &SettingsPage::handleSave);

        connect(mResetTasksBtn, &QPushButton::clicked,
                this, &SettingsPage::handleResetTasks);

        connect(mResetProjectsBtn, &QPushButton::clicked,
                this, &SettingsPage::handleResetProjects);

        connect(mMenuBtn, &QPushButton::clicked,
            this, &SettingsPage::onMenuClicked);


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
