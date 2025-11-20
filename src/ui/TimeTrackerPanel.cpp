#include "TimeTrackerPanel.h"

#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGuiApplication>
#include <QScreen>
#include <QLayout>

#include "SideTab.h"
#include "BaseCardPage.h"

namespace ui
{
    TimeTrackerPanel::TimeTrackerPanel(QWidget* parent)
        : QWidget{parent}
        , mOpen{false}
        , mOpenGeometry{QRect{}}
        , mClosedGeometry{QRect{}}
        , mAnimation{nullptr}
        , mCurrentPage{nullptr}
    {
        // Window flags: frameless + tool + stays on top
        setWindowFlags(Qt::FramelessWindowHint
            | Qt::Tool
            | Qt::WindowStaysOnTopHint);

        setAttribute(Qt::WA_TranslucentBackground);

        // Animation
        mAnimation = new QPropertyAnimation(this, "geometry", this);
        mAnimation->setDuration(250);
        mAnimation->setEasingCurve(QEasingCurve::OutCirc);

        // Setup calls
        updateTargetGeometries();
        setGeometry(mOpenGeometry);
        mOpen = true;

        // Layout to hold page
        auto* layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        setLayout(layout);
    }

    void TimeTrackerPanel::setPage(BaseCardPage* page)
    {
        if (mCurrentPage)
        {
            layout()->removeWidget(mCurrentPage);
            mCurrentPage->deleteLater();
        }

        mCurrentPage = page;

        if (page)
        {
            layout()->addWidget(page);
        }
    }

//     void TimeTrackerPanel::buildUi()
//     {
//         // Timer
//         auto* timerLabel = new QLabel("00:00:00", mainCard);
//         timerLabel->setObjectName("TimerLabel");
//         timerLabel->setAlignment(Qt::AlignCenter);
//
//         // Date
//         auto* dateLabel = new QLabel("11/18/2025", mainCard);
//         dateLabel->setObjectName("DateLabel");
//         dateLabel->setAlignment(Qt::AlignCenter);
//
//         // Project Title
//         auto* projectLabel = new QLabel("AI SKILLS FUNDAMENTALS\nCERTIFICATE", mainCard);
//         projectLabel->setObjectName("ProjectLabel");
//         projectLabel->setAlignment(Qt::AlignCenter);
//         projectLabel->setWordWrap(true);
//
//         // Group Date + Project
//         auto* dpGroup = new QVBoxLayout();
//         dpGroup->setSpacing(2);
//         dpGroup->setContentsMargins(0, 0, 0, 0);
//         dpGroup->addWidget(dateLabel);
//         dpGroup->addWidget(projectLabel);
//
//         // Group: Timer + dpGroup
//         auto* tdpGroup = new QVBoxLayout();
//         tdpGroup->setSpacing(0);
//         tdpGroup->setContentsMargins(0, 0, 0, 0);
//         tdpGroup->addWidget(timerLabel);
//         tdpGroup->addLayout(dpGroup);
//
//         // Task Title
//         mTaskLabel = new QLabel(
//             "Long task name appears like\n"
//             "this on the device UI screen\n"
//             "when it goes to three lines.",
//             mainCard
//         );
//         mTaskLabel->setObjectName("TaskLabel");
//         mTaskLabel->setAlignment(Qt::AlignCenter);
//         mTaskLabel->setWordWrap(true);
//
//         // Group: tdpGroup + Task
//         auto* tdptGroup = new QVBoxLayout();
//         tdptGroup->setSpacing(24);
//         tdptGroup->setContentsMargins(0, 0, 0, 0);
//         tdptGroup->addLayout(tdpGroup);
//         tdptGroup->addWidget(mTaskLabel);
//
//         // OLD STATUS ADDITION
//         // // Group: Status + tdpGroup
//         // auto* stdptGroup = new QVBoxLayout();
//         // stdptGroup->setSpacing(20);
//         // stdptGroup->setContentsMargins(0, 0, 0, 0);
//         // stdptGroup->addWidget(mTitleLabel);
//         // stdptGroup->addLayout(tdptGroup);
//
//         // Start Button
//         mStartBtn = new QPushButton("START", mMainCard);
//         mStartBtn->setObjectName("StartBtn");
//         mStartBtn->setCursor(QCursor(Qt::PointingHandCursor));
//         mStartBtn->setFixedHeight(44);
//         mStartBtn->setFixedWidth(220);
//         mStartBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//
//         // Back Button
//         mReturnBtn = new QPushButton("BACK", mMainCard);
//         mReturnBtn->setObjectName("ReturnBtn");
//         mReturnBtn->setCursor(QCursor(Qt::PointingHandCursor));
//         mReturnBtn->setFixedHeight(32);
//         mReturnBtn->setFixedWidth(84);
//         mStartBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//         mReturnBtn->setFlat(true);
//
//         auto* btnGroup = new QVBoxLayout();
//         btnGroup->setSpacing(10);
//         btnGroup->setContentsMargins(0, 0, 0, 0);
//         btnGroup->addWidget(mStartBtn, 0, Qt::AlignHCenter);
//         btnGroup->addWidget(mReturnBtn, 0, Qt::AlignHCenter);
//
//         // Lambda to set specific vertical sizing
//         auto fixedText = [](QLabel* label)
//         {
//             label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
//             label->setMargin(0);
//         };
//
//         fixedText(mTitleLabel);
//         fixedText(timerLabel);
//         fixedText(dateLabel);
//         fixedText(projectLabel);
//         fixedText(mTaskLabel);
//
//         // Layout Order
//         //mCardLayout->addWidget(mTitleLabel);
//         //mCardLayout->addSpacing(2);
//         //mCardLayout->addWidget(timerLabel);
//         //mCardLayout->addLayout(dpGroup);
//         //mCardLayout->addWidget(projectLabel);
//        // mCardLayout->addSpacing(8);
//         //mCardLayout->addWidget(mTaskLabel);
//         //mCardLayout->addSpacing(8);
//         //mCardLayout->addLayout(stdptGroup);
//         mCardLayout->addLayout(btnGroup);
//         //mCardLayout->addWidget(mReturnBtn);
//
//         mRootLayout->addWidget(mMainCard);
//
//
//         //region Stylesheet
//
//         setStyleSheet(R"(
//
//         )");
//         //endregion
//     }

    void TimeTrackerPanel::updateTargetGeometries()
    {
        const QScreen* screen = QGuiApplication::primaryScreen();
        if (!screen) return;

        const QRect avail = screen->availableGeometry();

        constexpr int panelWidth = 392;   // full width of panel
        constexpr int tabWidth = 40;      // how much remains visible when closed
        constexpr int panelHeight = 420;  // height of panel
        const int y = (avail.height() - panelHeight) / 2;

        // Fully visible (open)
        mOpenGeometry = QRect(avail.right() - panelWidth + 1, // +1 to stay on-screen
                         y,
                       panelWidth,
                       panelHeight);

        // Only tab visible (closed)
        mClosedGeometry = QRect(avail.right() - tabWidth + 10,
                                y,
                              panelWidth,
                              panelHeight); // width stays the same, just moved right
    }

    void TimeTrackerPanel::setOpen(const bool open)
    {
        if (mOpen == open) return;

        updateTargetGeometries(); // in case screen geometry changed

        mOpen = open;
        mAnimation->stop();

        mAnimation->setStartValue(geometry());
        mAnimation->setEndValue(open ? mOpenGeometry : mClosedGeometry);
        mAnimation->start();

        emit toggled(mOpen);
    }

    void TimeTrackerPanel::mousePressEvent(QMouseEvent* event)
    {
        // Very simple: clicking anywhere on the panel toggles it.
        // You can refine this to check if the click is only on the tab area.
        if (event->button() == Qt::LeftButton)
        {
            setOpen(!mOpen);
        }

        QWidget::mousePressEvent(event);
    }
}
