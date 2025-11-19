#pragma once

#include <QWidget>
#include <QPropertyAnimation>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>

#include "GradientFrame.h"

namespace ui
{
    class TimeTrackerPanel : public QWidget
    {
        Q_OBJECT

    public:
        explicit TimeTrackerPanel(QWidget* parent = nullptr);
        ~TimeTrackerPanel();

        // Open / Close
        void setOpen(bool open);

        bool isOpen() const { return mOpen; }

    signals:
        void toggled(bool open);

    protected:
        void mousePressEvent(QMouseEvent* event) override;

    private:
        void buildUi();
        void updateTargetGeometries();

        bool mOpen;
        QRect mOpenGeometry;
        QRect mClosedGeometry;
        QPropertyAnimation* mAnimation;

        QHBoxLayout* mRootLayout;
        QVBoxLayout* mCardLayout;

        class SideTab* mSideTab;
        GradientFrame* mMainCard;
        QGraphicsDropShadowEffect* mShadow;

        QLabel* mStatusLabel;
        QLabel* mTimerLabel;
        QLabel* mDateLabel;
        QLabel* mTaskLabel;
        QLabel* mProjectLabel;
        QPushButton* mStartBtn;
        QPushButton* mReturnBtn;

    };
} // ui