#pragma once

#include "BaseCardPage.h"

class QLabel;
class QPushButton;

namespace ui
{
    class TrackWorkTimerPage : public BaseCardPage
    {
        Q_OBJECT

    public:
        explicit TrackWorkTimerPage(QWidget* parent = nullptr);

        void setTimerText(const QString& text);
        void setDateText(const QString& text);
        void setProjectText(const QString& text);
        void setTaskText(const QString& text);

        QPushButton* getStartButton() const { return mStartBtn; }
        QPushButton* getReturnButton() const { return mReturnBtn; }

    signals:
        void startClicked();
        void returnClicked();

    private:
        QLabel* mTimerLabel;
        QLabel* mDateLabel;
        QLabel* mProjectLabel;
        QLabel* mTaskLabel;
        QPushButton* mStartBtn;
        QPushButton* mReturnBtn;

    };
} // ui