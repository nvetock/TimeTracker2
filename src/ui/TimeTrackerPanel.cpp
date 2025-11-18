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

        buildUi();

        // Animation
        mAnimation = new QPropertyAnimation(this, "geometry", this);
        mAnimation->setDuration(250);
        mAnimation->setEasingCurve(QEasingCurve::InOutQuad);

        // Setup calls
        updateTargetGeometries();
        setGeometry(mOpenGeometry);
        mOpen = true;
        
        // setAttribute(Qt::WA_TranslucentBackground);
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
        mRootLayout->addWidget(mSideTab);

        // --- Main card
        mMainCard = new QFrame{this};
        mMainCard->setObjectName("MainCard");
        mMainCard->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        mCardLayout = new QVBoxLayout{this};
        mCardLayout->setContentsMargins(24, 20, 24, 20);
        mCardLayout->setSpacing(10);
        
        // Status
        mStatusLabel = new QLabel("NOT STARTED", mMainCard);
        mStatusLabel->setObjectName("StatusLabel");
        mStatusLabel->setAlignment(Qt::AlignCenter);

        // Timer
        mTimerLabel = new QLabel("00:00:00", mMainCard);
        mTimerLabel->setObjectName("TimerLabel");
        mTimerLabel->setAlignment(Qt::AlignCenter);

        // Project Title
        mProjectLabel = new QLabel("AI SKILLS FUNDAMENTALS\nCERTIFICATE", mMainCard);
        mProjectLabel->setObjectName("ProjectLabel");
        mProjectLabel->setAlignment(Qt::AlignCenter);
        mProjectLabel->setWordWrap(true);

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

        // Start Button
        mStartBtn = new QPushButton("START", mMainCard);
        mStartBtn->setObjectName("StartBtn");
        mStartBtn->setCursor(QCursor(Qt::PointingHandCursor));
        mStartBtn->setFixedHeight(44);

        // Back Button
        mReturnBtn = new QPushButton("BACK", mMainCard);
        mReturnBtn->setObjectName("ReturnBtn");
        mReturnBtn->setCursor(QCursor(Qt::PointingHandCursor));
        mReturnBtn->setFixedHeight(24);

        // Layout Order
        mCardLayout->addWidget(mStatusLabel);
        mCardLayout->addSpacing(4);
        mCardLayout->addWidget(mTimerLabel);
        mCardLayout->addWidget(mDateLabel);
        mCardLayout->addWidget(mProjectLabel);
        mCardLayout->addSpacing(8);
        mCardLayout->addWidget(mTaskLabel);
        mCardLayout->addSpacing(8);
        mCardLayout->addWidget(mStartBtn);
        mCardLayout->addWidget(mReturnBtn);

        mRootLayout->addWidget(mMainCard);

        //region Stylesheet

        setStyleSheet(R"(
            QFrame#TimeCard {
                background-color: #181A1C;
                border-radius: 16px;
            }

            QLabel#StatusLabel {
                color: #A0A4AA;
                font-family: "Helvetica Neue", Arial, sans-serif;
                font-size: 10px;
                letter-spacing: 2px;
            }

            QLabel#TimerLabel {
                color: #FFFFFF;
                font-family: "SF Mono", "Consolas", monospace;
                font-size: 32px;
                font-weight: 600;
            }

            QLabel#DateLabel {
                color: #A0A4AA;
                font-family: "Helvetica Neue", Arial, sans-serif;
                font-size: 10px;
                margin-top: 4px;
            }

            QLabel#TaskTitleLabel {
                color: #B4FF6B; /* lime accent */
                font-family: "Helvetica Neue", Arial, sans-serif;
                font-size: 11px;
                font-weight: 700;
            }

            QLabel#DescriptionLabel {
                color: #D0D3D7;
                font-family: "Helvetica Neue", Arial, sans-serif;
                font-size: 10px;
            }

            QPushButton#StartButton {
                background-color: #B4FF6B;
                color: #111111;
                font-family: "Helvetica Neue", Arial, sans-serif;
                font-size: 14px;
                font-weight: 700;
                border-radius: 8px;
                border: none;
            }

            QPushButton#StartButton:hover {
                background-color: #A1F05D;
            }

            QPushButton#StartButton:pressed {
                background-color: #8BD54A;
            }

            QLabel#BackLabel {
                color: #A0A4AA;
                font-family: "Helvetica Neue", Arial, sans-serif;
                font-size: 10px;
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

        constexpr int panelWidth  = 320; // full width of your panel
        constexpr int tabWidth    = 40;  // how much remains visible when closed
        constexpr int panelHeight = 420; // height of the panel
        const int y               = (avail.height() - panelHeight) / 2;

        // Fully visible (open)
        mOpenGeometry = QRect(avail.right() - panelWidth + 1, // +1 to stay on-screen
                               y,
                               panelWidth,
                               panelHeight);

        // Only tab visible (closed)
        mClosedGeometry = QRect(avail.right() - tabWidth + 1,
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
