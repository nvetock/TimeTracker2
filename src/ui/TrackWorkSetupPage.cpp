#include "TrackWorkSetupPage.h"

#include <future>
#include <QDateEdit>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QCursor>
#include <QLabel>
#include "ClickableSvgWidget.h"
#include "UiElemStyler.h"

namespace ui
{
    TrackWorkSetupPage::TrackWorkSetupPage(QWidget* parent)
        : BaseCardPage{parent}
        , mDate{nullptr}
        , mTaskCombo{nullptr}
        , mProjectCombo{nullptr}
        , taskPopupShown{false}
        , projectPopupShown{false}
        , mDescriptionEdit{nullptr}
        , mReadyBtn{nullptr}
        , mMenuBtn{nullptr}
    {
        setTitle("TRACK WORK");

        auto* formLayout = new QFormLayout();
        setZeroMarginAndSpaceBetween(formLayout, 24);

        // Date
        mDate = new QLabel(QDate::currentDate().toString(), this);
        mDate->setObjectName("Date");

        // Task combo
        auto* taskRow = new QWidget(this);
        auto* taskLayout = new QHBoxLayout(taskRow);
        setZeroMarginAndSpaceBetween(taskLayout, 8);

        mTaskCombo = generateComboBox("TaskCombo", "Select or type task…", true, taskRow);
        auto* taskArrow = generateIcon(":/icons/resources/icons/arrow_down.svg", taskRow, "ComboArrowIcon", 16);
        taskLayout->addWidget(mTaskCombo);
        taskLayout->addWidget(taskArrow);


        // Project combo
        auto* projRow = new QWidget(this);
        auto* projLayout = new QHBoxLayout(projRow);
        setZeroMarginAndSpaceBetween(projLayout, 8);
        mProjectCombo = generateComboBox("ProjectCombo", "Select or type project…", true, projRow);

        auto* projArrow = generateIcon(":/icons/resources/icons/arrow_down.svg", projRow, "ComboArrowIcon", 16);
        projLayout->addWidget(mProjectCombo);
        projLayout->addWidget(projArrow);


        connect(taskArrow, &ClickableSvgWidget::clicked,
                mTaskCombo, &QComboBox::showPopup);
        connect(projArrow, &ClickableSvgWidget::clicked,
                mProjectCombo, &QComboBox::showPopup);


        // Description
        mDescriptionEdit = new InlineEditableLabel(this);
        mDescriptionEdit->setObjectName("DescriptionEdit");
        mDescriptionEdit->setText("Set a work description...");


        // Labels on the left, widgets on the right
        formLayout->addRow(tr("DATE"), mDate);
        formLayout->addRow(tr("TASK"), taskRow);
        formLayout->addRow(tr("PROJECT"), projRow);
        formLayout->addRow(QString(), mDescriptionEdit);

        auto* body = getBodyLayout();
        body->addLayout(formLayout);
        body->addStretch();

        // -- Footer
        auto* footer = getFooterLayout();

        mReadyBtn = generateButton("Ready", "PrimaryBtn", 52, false, this);
        mMenuBtn = generateButton("Menu", "SecondaryBtn", 32, true, this);

        auto* buttonGroup = new QVBoxLayout();
        setZeroMarginAndSpaceBetween(buttonGroup, 12);
        buttonGroup->addWidget(mReadyBtn, 0, Qt::AlignHCenter);
        buttonGroup->addWidget(mMenuBtn, 0, Qt::AlignHCenter);

        footer->addLayout(buttonGroup);
        footer->addSpacing(32);

        // -- Connections
        connect(mReadyBtn, &QPushButton::clicked, this, &TrackWorkSetupPage::handleReady);
        connect(mMenuBtn, &QPushButton::clicked, this, &BaseCardPage::menuRequested);

    }

    void TrackWorkSetupPage::setAvailableTasks(const QStringList& tasks)
    {
        rebuildTaskCombo(tasks);
    }

    void TrackWorkSetupPage::setAvailableProjects(const QStringList& projects)
    {
        rebuildProjectCombo(projects);
    }

    QString TrackWorkSetupPage::getDate() const
    {
        return mDate->text();
    }

    QString TrackWorkSetupPage::getSelectedTaskName() const
    {
        return mTaskCombo
            ? mTaskCombo->currentText().trimmed()
            : QString();
    }

    QString TrackWorkSetupPage::getSelectedProjectName() const
    {
        return mProjectCombo
            ? mProjectCombo->currentText().trimmed()
            : QString();
    }

    QString TrackWorkSetupPage::getDescription() const
    {
        return mDescriptionEdit
            ? mDescriptionEdit->getText().trimmed()
            : QString();
    }

    void TrackWorkSetupPage::rebuildTaskCombo(const QStringList& tasks)
    {
        if (!mTaskCombo) return;
        mTaskCombo->clear();

        // Normal task entries
        for (const auto& t : tasks)
        {
            const QString trimmed = t.trimmed();
            if (trimmed.isEmpty()) continue;

            mTaskCombo->addItem(trimmed);

            // default entried get empty kind
            if (mTaskCombo->count() > 0)
            {
                mTaskCombo->setCurrentIndex(0);
            }
            else
            {
                mTaskCombo->setCurrentIndex(-1);
            }
        }
    }

    void TrackWorkSetupPage::rebuildProjectCombo(const QStringList& projects)
    {
        if (!mProjectCombo) return;
        mProjectCombo->clear();

        // Normal task entries
        for (const auto& p : projects)
        {
            const QString trimmed = p.trimmed();
            if (trimmed.isEmpty()) continue;

            mProjectCombo->addItem(trimmed);
        }

        if (mProjectCombo->count() > 0)
        {
            mProjectCombo->setCurrentIndex(0);
        }
        else
        {
            mProjectCombo->setCurrentIndex(-1);
        }
    }

    void TrackWorkSetupPage::handleReady()
    {
        const QString date = getDate();
        const QString task = getSelectedTaskName();
        const QString project = getSelectedProjectName();
        const QString desc = getDescription();

        emit readyClicked(date, task, project, desc);
    }
} // ui