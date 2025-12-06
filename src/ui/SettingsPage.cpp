#include "SettingsPage.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QCursor>
#include <QLabel>

#include "InlineEditableLabel.h"
#include "UiElemStyler.h"

namespace ui
{
    SettingsPage::SettingsPage(QWidget* parent)
        : BaseCardPage(parent)
    {
        setTitle("SETTINGS");

        auto* body   = getBodyLayout();

        // Name
        auto* nameLabel = generateLabel("Name", "StatusLabel", "center", false, this);
        mNameEdit = new InlineEditableLabel{this};
        mNameEdit->setObjectName("InlineEditLabel");
        mNameEdit->setText("Your name (for logs)");

        auto* nameSection = new QVBoxLayout();
        setZeroMarginAndSpaceBetween(nameSection, 8);
        nameSection->setAlignment(Qt::AlignHCenter);
        nameSection->addWidget(nameLabel);
        nameSection->addWidget(mNameEdit);


        // Save directory + browse
        auto* saveDirRow = new QHBoxLayout();
        saveDirRow->setContentsMargins(0, 0, 0, 0);
        saveDirRow->setSpacing(4);

        mSaveDirEdit = new QLineEdit(this);
        mSaveDirEdit->setObjectName("InlineLineEdit");
        mSaveDirEdit->setPlaceholderText("Save directory for logs/CSV");

        mBrowseBtn = new QPushButton(tr("Browse"), this);
        mBrowseBtn->setObjectName("RadioBtn");
        mBrowseBtn->setCursor(Qt::PointingHandCursor);
        mBrowseBtn->setFixedHeight(28);

        saveDirRow->addWidget(mSaveDirEdit, 1);
        saveDirRow->addWidget(mBrowseBtn, 0);

        auto* saveDirLabel = generateLabel("File Location", "StatusLabel", "center", false, this);
        auto* saveSection = new QVBoxLayout();
        setZeroMarginAndSpaceBetween(saveSection, 8);
        saveSection->addWidget(saveDirLabel);
        saveSection->addLayout(saveDirRow);

        // Reset buttons row
        auto* resetRow = new QHBoxLayout();
        setZeroMarginAndSpaceBetween(resetRow, 8);
        resetRow->setAlignment(Qt::AlignHCenter);

        auto* resetSection = new QVBoxLayout();
        setZeroMarginAndSpaceBetween(resetSection, 4);
        auto* resetLabel = generateLabel("Reset", "StatusLabel", "center", false, this);
        resetSection->addWidget(resetLabel);
        resetSection->addLayout(resetRow);


        mResetTasksBtn = generateButton("Tasks", "RadioBtn", 32, this);
        mResetProjectsBtn = generateButton("Projects", "RadioBtn", 32, this);

        resetRow->addWidget(mResetTasksBtn);
        resetRow->addWidget(mResetProjectsBtn);

        body->addSpacing(8);
        body->addLayout(nameSection);
        body->addSpacing(24);
        body->addLayout(saveSection);
        body->addSpacing(32);
        body->addLayout(resetSection);

        mSaveBtn = generateButton("Save", "PrimaryBtn", 44, this);
        mMenuBtn = generateButton("Menu", "SecondaryBtn", true, this);

        auto* btnLayout = new QVBoxLayout();
        setZeroMarginAndSpaceBetween(btnLayout, 8);
        btnLayout->addWidget(mSaveBtn, 0, Qt::AlignHCenter);
        btnLayout->addWidget(mMenuBtn, 0, Qt::AlignHCenter);

        auto* footer = getFooterLayout();
        footer->addLayout(btnLayout);
        footer->addSpacing(16);

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
        settings.userName      = mNameEdit    ? mNameEdit->getText().trimmed()      : QString{};
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
