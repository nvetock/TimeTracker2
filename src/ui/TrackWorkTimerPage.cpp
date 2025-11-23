#include "TrackWorkTimerPage.h"

#include <QDate>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTimer>

namespace ui
{
    TrackWorkTimerPage::TrackWorkTimerPage(QWidget* parent)
        : BaseCardPage{parent}
        , mTimerLabel{nullptr}
        , mDateLabel{nullptr}
        , mProjectLabel{nullptr}
        , mTaskLabel{nullptr}
        , mDescription{nullptr}
        , mStartBtn{nullptr}
        , mPauseBtn{nullptr}
        , mStopBtn{nullptr}
        , mReturnBtn{nullptr}
        , mUiTimer{nullptr}
        , mElapsedSeconds{0}
        , mIsPaused{false}
    {
        setTitle("NOT STARTED");
        showBackButton(false);

        auto* body = getBodyLayout();

        mTimerLabel = new QLabel("00:00:00", this);
        mTimerLabel->setObjectName("TimerLabel");
        mTimerLabel->setAlignment(Qt::AlignCenter);

        // Date
        QDate date = QDate::currentDate();
        mDateLabel = new QLabel(date.toString(), this);
        mDateLabel->setObjectName("DateLabel");
        mDateLabel->setAlignment(Qt::AlignCenter);

        // Project Title
        mProjectLabel = new QLabel("AI SKILLS FUNDAMENTALS\nCERTIFICATE", this);
        mProjectLabel->setObjectName("ProjectLabel");
        mProjectLabel->setAlignment(Qt::AlignCenter);
        mProjectLabel->setWordWrap(true);

        // Group Date + Project
        auto* dpGroup = new QVBoxLayout();
        dpGroup->setSpacing(2);
        dpGroup->setContentsMargins(0, 0, 0, 0);
        dpGroup->addWidget(mDateLabel);
        dpGroup->addWidget(mProjectLabel);

        // Group Timer + dpGroup
        auto* tdpGroup = new QVBoxLayout();
        tdpGroup->setSpacing(0);
        tdpGroup->setContentsMargins(0, 0, 0, 0);
        tdpGroup->addWidget(mTimerLabel);
        tdpGroup->addLayout(dpGroup);

        // Task Title
        mTaskLabel = new QLabel(
    "Long task name appears like\n"
            "this on the device UI screen\n"
            "when it goes to three lines.",
            this
        );
        mTaskLabel->setObjectName("TaskLabel");
        mTaskLabel->setAlignment(Qt::AlignCenter);
        mTaskLabel->setWordWrap(true);

        mDescription = new QLineEdit("default description.", this);
        mDescription->setObjectName("Description");
        mDescription->setAlignment(Qt::AlignCenter);

        auto* tdpttGroup = new QVBoxLayout();
        tdpttGroup->setSpacing(24);
        tdpttGroup->setContentsMargins(0, 0, 0, 0);
        tdpttGroup->addLayout(tdpGroup);
        tdpttGroup->addWidget(mTaskLabel);
        tdpttGroup->addWidget(mDescription);

        // Add to body area
        body->addLayout(tdpttGroup);
        auto fixedText = [](QLabel* label)
        {
            label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
            label->setMargin(0);
        };

        fixedText(mTimerLabel);
        fixedText(mDateLabel);
        fixedText(mProjectLabel);
        fixedText(mTaskLabel);

        // -- Footer
        auto* footer = getFooterLayout();

        mStartBtn = new QPushButton("START", this);
        mStartBtn->setObjectName("PrimaryBtn");
        mStartBtn->setCursor(QCursor(Qt::PointingHandCursor));
        //mStartBtn->setFixedWidth(220);
        mStartBtn->setFixedHeight(44);
        mStartBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        mPauseBtn = new QPushButton("PAUSE", this);
        mPauseBtn->setObjectName("PrimaryBtn");
        mPauseBtn->setVisible(false);

        mStopBtn = new QPushButton("STOP", this);
        mStopBtn->setObjectName("SecondaryBtn");
        mStopBtn->setFlat(true);
        mStopBtn->setVisible(false);

        mReturnBtn = new QPushButton("BACK", this);
        mReturnBtn->setObjectName("SecondaryBtn");
        mReturnBtn->setCursor(QCursor(Qt::PointingHandCursor));
        //mReturnBtn->setFixedHeight(32);
        //mReturnBtn->setFixedWidth(84);
        mReturnBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        mReturnBtn->setFlat(true);

        auto* btnGroup = new QVBoxLayout();
        btnGroup->setSpacing(10);
        btnGroup->setContentsMargins(0, 0, 0, 0);
        btnGroup->addWidget(mStartBtn, 0, Qt::AlignHCenter);
        btnGroup->addWidget(mReturnBtn, 0, Qt::AlignHCenter);
        btnGroup->addWidget(mPauseBtn, 0, Qt::AlignHCenter);
        btnGroup->addWidget(mStopBtn, 0, Qt::AlignHCenter);

        footer->addLayout(btnGroup);

        // TIMER
        mUiTimer = new QTimer(this);
        mUiTimer->setInterval(1000);
        connect(mUiTimer, &QTimer::timeout,
            this, &TrackWorkTimerPage::onUiTick);


        // CONNECTIONS
        connect(mStartBtn, &QPushButton::clicked,
                this, &TrackWorkTimerPage::onStartButtonClicked);

        connect(mPauseBtn, &QPushButton::clicked,
            this, &TrackWorkTimerPage::onPauseButtonClicked);

        connect(mStopBtn, &QPushButton::clicked,
            this, &TrackWorkTimerPage::onStopButtonClicked);

        connect(mReturnBtn, &QPushButton::clicked,
                this, &TrackWorkTimerPage::onReturnButtonClicked);
    }

    void TrackWorkTimerPage::applySessionStatus(timetracker::WorkSession::Status status)
    {
        using Status = timetracker::WorkSession::Status;
        switch (status)
        {
        case Status::Running:
            if (mUiTimer && !mUiTimer->isActive()) mUiTimer->start();
            setTitle("RUNNING");
            mIsPaused = false;
            setRecordingActive(true);
            setIdlePaused(false);
            setEnded(false);

            mStartBtn->setVisible(false);
            mReturnBtn->setVisible(false);
            mPauseBtn->setVisible(true);
            mStopBtn->setVisible(true);
            mPauseBtn->setText("PAUSE");
            break;
        case Status::Paused:
            if (mUiTimer && mUiTimer->isActive()) mUiTimer->stop();
            setTitle("PAUSED");
            mIsPaused = true;
            setRecordingActive(false);
            setIdlePaused(true);
            mPauseBtn->setText("RESUME");
            break;
        case Status::Completed:
            if (mUiTimer && mUiTimer->isActive()) mUiTimer->stop();
            setTitle("COMPLETED");
            mIsPaused = false;
            setRecordingActive(false);
            setIdlePaused(false);
            setEnded(true);

            mStartBtn->setVisible(false);
            mReturnBtn->setVisible(true);
            mPauseBtn->setVisible(false);
            mStopBtn->setVisible(false);
            break;
        case Status::Timeout:
            if (mUiTimer && mUiTimer->isActive()) mUiTimer->stop();
            setTitle("TIMEOUT");
            mIsPaused = false;
            setRecordingActive(false);
            setIdlePaused(false);
            setEnded(true);

            mStartBtn->setVisible(false);
            mReturnBtn->setVisible(true);
            mPauseBtn->setVisible(false);
            mStopBtn->setVisible(false);
        default:
            break;
        }
    }

    void TrackWorkTimerPage::onStartButtonClicked()
    {
        mElapsedSeconds = 0;
        updateTimerLabel();
        emit startClicked();
    }

    void TrackWorkTimerPage::onPauseButtonClicked()
    {
        emit pauseClicked();
    }

    void TrackWorkTimerPage::onStopButtonClicked()
    {
        emit stopClicked();
    }

    void TrackWorkTimerPage::onReturnButtonClicked()
    {
        emit returnClicked();
    }

    void TrackWorkTimerPage::onUiTick()
    {
        ++mElapsedSeconds;
        updateTimerLabel();
    }

    void TrackWorkTimerPage::updateTimerLabel()
    {
        const QTime t(0,0,0);
        const QString text = t.addSecs(mElapsedSeconds).toString("hh:mm:ss");
        setTimerText(text);
    }

    void TrackWorkTimerPage::setTimerText(const QString& text)
    {
        if (mTimerLabel) mTimerLabel->setText(text);
    }

    void TrackWorkTimerPage::setDateText(const QString& text)
    {
        if (mDateLabel) mDateLabel->setText(text);
    }

    void TrackWorkTimerPage::setProjectText(const QString& text)
    {
        if (mProjectLabel) mProjectLabel->setText(text);
    }

    void TrackWorkTimerPage::setTaskText(const QString& text)
    {
        if (mTaskLabel) mTaskLabel->setText(text);
    }

    void TrackWorkTimerPage::setDescriptionText(const QString& text)
    {
        if (mDescription) mDescription->setText(text);
    }
} // ui