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
        formLayout->setSpacing(0);

        // Date
        mDate = new QLabel(QDate::currentDate().toString(), this);
        mDate->setObjectName("Date");

        // Task combo
        mTaskCombo = new QComboBox(this);
        mTaskCombo->setObjectName("TaskCombo");
        mTaskCombo->setEditable(false);

        // Project combo
        mProjectCombo = new QComboBox(this);
        mProjectCombo->setObjectName("ProjectCombo");
        mProjectCombo->setEditable(false);

        // Description
        mDescriptionEdit = new QLineEdit(this);
        mDescriptionEdit->setObjectName("DescriptionEdit");
        mDescriptionEdit->setPlaceholderText("Work description (optional)");

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
        mReadyBtn->setFixedWidth(44);

        mBackBtn = new QPushButton(tr("BACK"), this);
        mBackBtn->setObjectName("ReturnBtn");
        mBackBtn->setCursor(Qt::PointingHandCursor);
        mBackBtn->setFlat(true);

        auto* buttonGroup = new QVBoxLayout();
        buttonGroup->setContentsMargins(0,0,0,0);
        buttonGroup->setSpacing(8);
        buttonGroup->addWidget(mReadyBtn, 0, Qt::AlignHCenter);
        buttonGroup->addWidget(mBackBtn, 0, Qt::AlignHCenter);

        footer->addLayout(buttonGroup);

        // -- Connections
        connect(mReadyBtn, &QPushButton::clicked, this, &TrackWorkSetupPage::handleReady);
        connect(mBackBtn, &QPushButton::clicked, this, &TrackWorkSetupPage::backRequested);

        connect(mTaskCombo, qOverload<int>(&QComboBox::currentIndexChanged),
            this, &TrackWorkSetupPage::handleTaskChanged);

        connect(mProjectCombo, qOverload<int>(&QComboBox::currentIndexChanged),
            this, &TrackWorkSetupPage::handleProjectChanged);

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
        if (!mTaskCombo) return {};

        const int idx = mTaskCombo->currentIndex();
        if (idx < 0) return {};

        const auto kind = mTaskCombo->itemData(idx, RoleKind).toString();
        if (kind == QStringLiteral("add-task")) return {};

        return mTaskCombo->currentText().trimmed();
    }

    QString TrackWorkSetupPage::getSelectedProjectName() const
    {
        if (!mProjectCombo) return {};

        const int idx = mProjectCombo->currentIndex();
        if (idx < 0) return {};

        const auto kind = mProjectCombo->itemData(idx, RoleKind).toString();
        if (kind == QStringLiteral("add-task")) return {};

        return mProjectCombo->currentText().trimmed();
    }

    QString TrackWorkSetupPage::getDescription() const
    {
        return mDescriptionEdit
            ? mDescriptionEdit->text().trimmed()
            : QString();
    }

    void TrackWorkSetupPage::rebuildTaskCombo(const QStringList& tasks)
    {
        mTaskCombo->clear();

        // Normal task entries
        for (const auto& t : tasks)
        {
            const QString trimmed = t.trimmed();
            if (trimmed.isEmpty()) continue;

            mTaskCombo->addItem(trimmed);

            // default entried get empty kind
            mTaskCombo->setItemData(mTaskCombo->count() - 1, QString(), RoleKind);
        }

        // Special 'Add new..' task appended
        const int addIdx = mTaskCombo->count();
        mTaskCombo->addItem(tr("Add new task..."));
        mTaskCombo->setItemData(addIdx, QStringLiteral("add-task"), RoleKind);

        // Start with no selection
        if (tasks.isEmpty())
        {
            mTaskCombo->setCurrentIndex(addIdx);
        }
        else
        {
            mTaskCombo->setCurrentIndex(0);
        }
    }

    void TrackWorkSetupPage::rebuildProjectCombo(const QStringList& projects)
    {
        mProjectCombo->clear();

        // Normal task entries
        for (const auto& p : projects)
        {
            const QString trimmed = p.trimmed();
            if (trimmed.isEmpty()) continue;

            mProjectCombo->addItem(trimmed);

            // default entried get empty kind
            mProjectCombo->setItemData(mProjectCombo->count() - 1, QString(), RoleKind);
        }

        // Special 'Add new..' task appended
        const int addIdx = mProjectCombo->count();
        mProjectCombo->addItem(tr("Add new project..."));
        mProjectCombo->setItemData(addIdx, QStringLiteral("add-project"), RoleKind);

        // Start with no selection
        if (projects.isEmpty())
        {
            mProjectCombo->setCurrentIndex(addIdx);
        }
        else
        {
            mProjectCombo->setCurrentIndex(0);
        }
    }


    void TrackWorkSetupPage::handleTaskChanged(int index)
    {
        if (!mTaskCombo || index < 0) return;

        const auto kind = mTaskCombo->itemData(index, RoleKind).toString();
        if (kind == QStringLiteral("add-task"))
        {
            // Emit to allow controller to open dialog / settings view
            // to handle this addition
            emit addNewTaskRequested();
        }
    }

    void TrackWorkSetupPage::handleProjectChanged(int index)
    {
        if (!mProjectCombo || index < 0) return;

        const auto kind = mProjectCombo->itemData(index, RoleKind).toString();
        if (kind == QStringLiteral("add-project"))
        {
            // Emit to allow controller to open dialog / settings view
            // to handle this addition
            emit addNewProjectRequested();
        }
    }

    void TrackWorkSetupPage::handleReady()
    {
        emit readyClicked(getDate(),
            getSelectedTaskName(),
            getSelectedProjectName(),
            getDescription()
        );
    }
} // ui