#include "TimeTrackerPanel.h"

#include <QGuiApplication>
#include <QScreen>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "SideTab.h"

namespace ui
{
    TimeTrackerPanel::TimeTrackerPanel(QWidget* parent)
        : QWidget(parent)
          , mOpen{false}
          , mOpenGeometry{QRect{}}
          , mClosedGeometry{QRect{}}
          , mAnimation{nullptr}
          , mRootLayout{nullptr}
          , mCardLayout{nullptr}
          , mSideTab{nullptr}
          , mMainCard{nullptr}
          , mStatusLabel{nullptr}
          , mTimerLabel{nullptr}
          , mDateLabel{nullptr}
          , mTaskLabel{nullptr}
          , mProjectLabel{nullptr}
          , mStartBtn{nullptr}
          , mReturnBtn{nullptr}
    {
        // Window flags: frameless + tool + stays on top
        setWindowFlags(Qt::FramelessWindowHint
            | Qt::Tool
            | Qt::WindowStaysOnTopHint);

        setAttribute(Qt::WA_TranslucentBackground);

        buildUi();

        // Animation
        mAnimation = new QPropertyAnimation(this, "geometry", this);
        mAnimation->setDuration(250);
        mAnimation->setEasingCurve(QEasingCurve::OutCirc);

        // Setup calls
        updateTargetGeometries();
        setGeometry(mOpenGeometry);
        mOpen = true;
    }

    TimeTrackerPanel::~TimeTrackerPanel()
    {
        delete mAnimation;
        delete mRootLayout;
        delete mCardLayout;
        delete mSideTab;
        delete mMainCard;
        delete mStatusLabel;
        delete mTimerLabel;
        delete mDateLabel;
        delete mTaskLabel;
        delete mProjectLabel;
        delete mStartBtn;
        delete mReturnBtn;
    }

    void TimeTrackerPanel::buildUi()
    {
        mRootLayout = new QHBoxLayout{this};
        mRootLayout->setContentsMargins(0, 0, 0, 0);
        mRootLayout->setSpacing(0);

        // --- Side tab
        mSideTab = new SideTab{this};
        mSideTab->setObjectName("SideTab");
        mSideTab->setFixedSize(32, 152);
        mRootLayout->addWidget(mSideTab, 0, Qt::AlignTop);

        // --- Main card
        mMainCard = new GradientFrame{this};
        mMainCard->setObjectName("MainCard");
        // Set the main panel size
        mMainCard->setFixedSize(360, 550);

        mCardLayout = new QVBoxLayout{mMainCard};
        mCardLayout->setContentsMargins(24, 20, 24, 20);
        mCardLayout->setSpacing(0);

        mShadow = new QGraphicsDropShadowEffect(mMainCard);
        mShadow->setBlurRadius(48);
        mShadow->setOffset(0, -12);
        mShadow->setColor(QColor(0, 0, 0, 100));
        mMainCard->setGraphicsEffect(mShadow);

        // Status
        mStatusLabel = new QLabel("NOT STARTED", mMainCard);
        mStatusLabel->setObjectName("StatusLabel");
        mStatusLabel->setAlignment(Qt::AlignCenter);

        // Timer
        mTimerLabel = new QLabel("00:00:00", mMainCard);
        mTimerLabel->setObjectName("TimerLabel");
        mTimerLabel->setAlignment(Qt::AlignCenter);

        // Date
        mDateLabel = new QLabel("11/18/2025", mMainCard);
        mDateLabel->setObjectName("DateLabel");
        mDateLabel->setAlignment(Qt::AlignCenter);

        // Project Title
        mProjectLabel = new QLabel("AI SKILLS FUNDAMENTALS\nCERTIFICATE", mMainCard);
        mProjectLabel->setObjectName("ProjectLabel");
        mProjectLabel->setAlignment(Qt::AlignCenter);
        mProjectLabel->setWordWrap(true);

        // Group Date + Project
        auto* dpGroup = new QVBoxLayout();
        dpGroup->setSpacing(2);
        dpGroup->setContentsMargins(0, 0, 0, 0);
        dpGroup->addWidget(mDateLabel);
        dpGroup->addWidget(mProjectLabel);

        // Group: Timer + dpGroup
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
            mMainCard
        );
        mTaskLabel->setObjectName("TaskLabel");
        mTaskLabel->setAlignment(Qt::AlignCenter);
        mTaskLabel->setWordWrap(true);

        // Group: tdpGroup + Task
        auto* tdptGroup = new QVBoxLayout();
        tdptGroup->setSpacing(24);
        tdptGroup->setContentsMargins(0, 0, 0, 0);
        tdptGroup->addLayout(tdpGroup);
        tdptGroup->addWidget(mTaskLabel);

        // Group: Status + tdpGroup
        auto* stdptGroup = new QVBoxLayout();
        stdptGroup->setSpacing(20);
        stdptGroup->setContentsMargins(0, 0, 0, 0);
        stdptGroup->addWidget(mStatusLabel);
        stdptGroup->addLayout(tdptGroup);

        // Start Button
        mStartBtn = new QPushButton("START", mMainCard);
        mStartBtn->setObjectName("StartBtn");
        mStartBtn->setCursor(QCursor(Qt::PointingHandCursor));
        mStartBtn->setFixedHeight(44);
        mStartBtn->setFixedWidth(220);
        mStartBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        // Back Button
        mReturnBtn = new QPushButton("BACK", mMainCard);
        mReturnBtn->setObjectName("ReturnBtn");
        mReturnBtn->setCursor(QCursor(Qt::PointingHandCursor));
        mReturnBtn->setFixedHeight(32);
        mReturnBtn->setFixedWidth(84);
        mStartBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        mReturnBtn->setFlat(true);

        auto* btnGroup = new QVBoxLayout();
        btnGroup->setSpacing(10);
        btnGroup->setContentsMargins(0, 0, 0, 0);
        btnGroup->addWidget(mStartBtn, 0, Qt::AlignHCenter);
        btnGroup->addWidget(mReturnBtn, 0, Qt::AlignHCenter);

        // Lambda to set specific vertical sizing
        auto fixedText = [](QLabel* label)
        {
            label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
            label->setMargin(0);
        };

        fixedText(mStatusLabel);
        fixedText(mTimerLabel);
        fixedText(mDateLabel);
        fixedText(mProjectLabel);
        fixedText(mTaskLabel);

        // Layout Order
        //mCardLayout->addWidget(mStatusLabel);
        //mCardLayout->addSpacing(2);
        //mCardLayout->addWidget(mTimerLabel);
        //mCardLayout->addLayout(dpGroup);
        //mCardLayout->addWidget(mProjectLabel);
       // mCardLayout->addSpacing(8);
        //mCardLayout->addWidget(mTaskLabel);
        //mCardLayout->addSpacing(8);
        mCardLayout->addLayout(stdptGroup);
        mCardLayout->addLayout(btnGroup);
        //mCardLayout->addWidget(mReturnBtn);

        mRootLayout->addWidget(mMainCard);

        //region Stylesheet

        setStyleSheet(R"(
            QFrame#MainCard {
                background-color: #262626;
                background-image: linear-gradient(#262626, #ffffff);
                border-bottom-left-radius: 16px;
            }

            QLabel#StatusLabel {
                color: #A0A4AA;
                font-family: "Barlow Semi Condensed", Arial, sans-serif;
                font-size: 16px;
                letter-spacing: 2px;
                margin: 0;
            }

            QLabel#TimerLabel {
                color: #FFFFFF;
                font-family: "Barlow Condensed", "Consolas", monospace;
                font-size: 72px;
                font-weight: 600;
                margin: 0;
            }

            QLabel#DateLabel {
                color: #FFFFFF;
                font-family: "Barlow Semi Condensed", Arial, sans-serif;
                letter-spacing: 2px;
                font-size: 18px;
                margin-top: 4px;
            }

            QLabel#ProjectLabel {
                color: #00E099;
                font-family: "Barlow Semi Condensed", Arial, sans-serif;
                font-size: 22px;
                letter-spacing: 1px;
                font-weight: 700;
            }

            QLabel#TaskLabel {
                color: #D0D3D7;
                font-family: "Barlow", Arial, sans-serif;
                font-weight: 600;
                letter-spacing: 1px;
                font-size: 18px;
            }

            QPushButton#StartBtn {
                background-color: #00E099;
                color: #262626;
                font-family: "Barlow Semi Condensed", Arial, sans-serif;
                font-size: 40px;
                padding-bottom: 4px;
                font-weight: 700;
                border-radius: 8px;
                border: none;
            }

            QPushButton#StartBtn:hover {
                background-color: #00F5C6;
            }

            QPushButton#StartBtn:pressed {
                background-color: #00C385;
            }

            QPushButton#ReturnBtn {
                background-color: transparent;
                background: transparent;
                border: none;
                color: #00E099;
                font-size: 24px;
                font-family: "Barlow Semi Condensed", Arial, sans-serif;
                font-weight: 700;
            }

            QPushButton#ReturnBtn:hover {
                color: #00F5C6;
            }
        )");
        //endregion
    }

    void TimeTrackerPanel::updateTargetGeometries()
    {
        const QScreen* screen = QGuiApplication::primaryScreen();
        if (!screen)
            return;

        const QRect avail = screen->availableGeometry();

        constexpr int panelWidth = 392; // full width of your panel
        constexpr int tabWidth = 40; // how much remains visible when closed
        constexpr int panelHeight = 420; // height of the panel
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

    void TimeTrackerPanel::setOpen(bool open)
    {
        if (mOpen == open)
            return;

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
