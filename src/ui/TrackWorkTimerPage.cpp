#include "TrackWorkTimerPage.h"

#include <QDate>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace ui
{
    TrackWorkTimerPage::TrackWorkTimerPage(QWidget* parent)
        : BaseCardPage{parent}
        , mTimerLabel{nullptr}
        , mDateLabel{nullptr}
        , mProjectLabel{nullptr}
        , mTaskLabel{nullptr}
        , mStartBtn{nullptr}
        , mReturnBtn{nullptr}
    {
        setTitle("NOT STARTED");
        showBackButton(false);

        auto* body = getBodyLayout();

        mTimerLabel = new QLabel("00:00:00", this);
        mTimerLabel->setObjectName("mTimerLabel");
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

        auto* tdptGroup = new QVBoxLayout();
        tdptGroup->setSpacing(24);
        tdptGroup->setContentsMargins(0, 0, 0, 0);
        tdptGroup->addLayout(tdpGroup);
        tdptGroup->addWidget(mTaskLabel);

        // Add to body area
        body->addLayout(tdptGroup);
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
        mStartBtn->setObjectName("StartButton");
        mStartBtn->setCursor(QCursor(Qt::PointingHandCursor));
        mStartBtn->setFixedHeight(220);
        mStartBtn->setFixedHeight(44);
        mStartBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        mReturnBtn = new QPushButton("BACK", this);
        mReturnBtn->setObjectName("ReturnButton");
        mReturnBtn->setCursor(QCursor(Qt::PointingHandCursor));
        mReturnBtn->setFixedHeight(32);
        mReturnBtn->setFixedWidth(84);
        mReturnBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        mReturnBtn->setFlat(true);

        auto* btnGroup = new QVBoxLayout();
        btnGroup->setSpacing(10);
        btnGroup->setContentsMargins(0, 0, 0, 0);
        btnGroup->addWidget(mStartBtn, 0, Qt::AlignHCenter);
        btnGroup->addWidget(mReturnBtn, 0, Qt::AlignHCenter);

        footer->addLayout(btnGroup);

        connect(mStartBtn, &QPushButton::clicked,
                this, &TrackWorkTimerPage::startClicked);

        connect(mReturnBtn, &QPushButton::clicked,
                this, &TrackWorkTimerPage::returnClicked);
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
} // ui