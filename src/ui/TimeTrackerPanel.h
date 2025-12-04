#pragma once

#include <QWidget>
#include <QPropertyAnimation>

#include "GradientFrame.h"

namespace ui
{
    class BaseCardPage;

    class TimeTrackerPanel : public QWidget
    {
        Q_OBJECT

    public:
        explicit TimeTrackerPanel(QWidget* parent = nullptr);
        ~TimeTrackerPanel() override = default;

        // Open / Close
        void setOpen(bool open);
        bool isOpen() const { return mOpen; }

        /**
         * Replace the current page inside the panel
         * @param page Next page to replace the current.
         */
        void setPage(BaseCardPage* page);
        BaseCardPage* getPage() const { return mCurrentPage;}

    signals:
        void toggled(bool open);

    private:
        void updateTargetGeometries();

        bool mOpen;
        QRect mOpenGeometry;
        QRect mClosedGeometry;
        QPropertyAnimation* mAnimation;

        BaseCardPage* mCurrentPage;
    };
} // ui