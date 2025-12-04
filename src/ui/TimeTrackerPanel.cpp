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

        // Connect the SideTab clicking to open/close the panel
        const SideTab* tab = getPage()->getSideTab();
        if (tab)  // If null ignore connection, otherwise establish.
        {
            connect(tab, &SideTab::clicked,
                this, [this]()
                {
                    setOpen(!mOpen);
                });
        }
    }

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
}
