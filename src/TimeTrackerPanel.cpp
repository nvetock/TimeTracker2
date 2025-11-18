#include "ui/TimeTrackerPanel.h"

#include <QGuiApplication>
#include <QScreen>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

namespace ui
{
    TimeTrackerPanel::TimeTrackerPanel(QWidget* parent)
        : QWidget(parent)
        , mTitle{nullptr}
        , mStartBtn{nullptr}
    {
        // Window flags: frameless + tool + stays on top
        setWindowFlags(Qt::FramelessWindowHint
                       | Qt::Tool
                       | Qt::WindowStaysOnTopHint);

        // Optional if you want rounded corners & custom background:
        // setAttribute(Qt::WA_TranslucentBackground);

        // Simple placeholder layout (replace with your real UI)
        mLayout = new QVBoxLayout(this);
        mLayout->setContentsMargins(20, 20, 20, 20);

        mTitle = new QLabel("TIME TRACKER", this);
        mStartBtn = new QPushButton("START", this);
        mLayout->addWidget(mTitle);
        mLayout->addStretch();
        mLayout->addWidget(mStartBtn);

        // Animation that moves the window
        mAnimation = new QPropertyAnimation(this, "geometry", this);
        mAnimation->setDuration(250);
        mAnimation->setEasingCurve(QEasingCurve::InOutQuad);

        updateTargetGeometries();

        // Start in open state
        setGeometry(mOpenGeometry);
        mOpen = true;

        // You can connect mStartBtn to your AppController later
    }

    TimeTrackerPanel::~TimeTrackerPanel()
    {
        delete mTitle;
        delete mStartBtn;
        delete mLayout;
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
