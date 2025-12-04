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

namespace ui
{
    TrackWorkSetupPage::TrackWorkSetupPage(QWidget* parent)
        : BaseCardPage{parent}
        , mDate{nullptr}
        , mTaskCombo{nullptr}
        , mProjectCombo{nullptr}
        , mDescriptionEdit{nullptr}
        , mReadyBtn{nullptr}
        , mBackBtn{nullptr}
    {
        setTitle("TRACK WORK");
        showBackButton(true);

        // -- Body
        auto* body = getBodyLayout();

        auto* formLayout = new QFormLayout();
        formLayout->setContentsMargins(0,0,0,0);
        formLayout->setSpacing(24);

        // Date
        mDate = new QLabel(QDate::currentDate().toString(), this);
        mDate->setObjectName("Date");

        // Task combo
        mTaskCombo = new QComboBox(this);
        mTaskCombo->setObjectName("TaskCombo");
        mTaskCombo->setEditable(true);
        mTaskCombo->lineEdit()->setPlaceholderText("Select or type task…");

        // Project combo
        mProjectCombo = new QComboBox(this);
        mProjectCombo->setObjectName("ProjectCombo");
        mProjectCombo->setEditable(true);
        mProjectCombo->lineEdit()->setPlaceholderText("Select or type project…");

        mTaskCombo->setInsertPolicy(QComboBox::NoInsert);
        mProjectCombo->setInsertPolicy(QComboBox::NoInsert);

        // Description
        mDescriptionEdit = new InlineEditableLabel(this);
        mDescriptionEdit->setObjectName("DescriptionEdit");
        mDescriptionEdit->setText("Set a work description...");


        // Labels on the left, widgets on the right
        formLayout->addRow(tr("DATE"), mDate);
        formLayout->addRow(tr("TASK"), mTaskCombo);
        formLayout->addRow(tr("PROJECT"), mProjectCombo);
        formLayout->addRow(QString(), mDescriptionEdit);

        body->addLayout(formLayout);
        body->addStretch();

        // -- Footer
        auto* footer = getFooterLayout();

        mReadyBtn = new QPushButton(tr("READY"), this);
        mReadyBtn->setObjectName("PrimaryBtn");
        mReadyBtn->setCursor(Qt::PointingHandCursor);
        mReadyBtn->setFixedHeight(44);
        mReadyBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        //mReadyBtn->setFixedWidth(44);

        mBackBtn = new QPushButton(tr("BACK"), this);
        mBackBtn->setObjectName("SecondaryBtn");
        mBackBtn->setCursor(Qt::PointingHandCursor);
        mBackBtn->setFlat(true);

        auto* buttonGroup = new QVBoxLayout();
        buttonGroup->setContentsMargins(0,0,0,0);
        buttonGroup->setSpacing(12);
        buttonGroup->addWidget(mReadyBtn, 0, Qt::AlignHCenter);
        buttonGroup->addWidget(mBackBtn, 0, Qt::AlignHCenter);

        footer->addLayout(buttonGroup);
        footer->addSpacing(32);

        // -- Connections
        connect(mReadyBtn, &QPushButton::clicked, this, &TrackWorkSetupPage::handleReady);
        connect(mBackBtn, &QPushButton::clicked, this, &TrackWorkSetupPage::backRequested);

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