#pragma once

#include <QWidget>
#include <QPropertyAnimation>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

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
        void updateTargetGeometries();

        bool mOpen;
        QRect mOpenGeometry;
        QRect mClosedGeometry;
        QPropertyAnimation* mAnimation;

        QVBoxLayout* mLayout;
        QLabel* mTitle;
        QPushButton* mStartBtn;

    };
} // ui