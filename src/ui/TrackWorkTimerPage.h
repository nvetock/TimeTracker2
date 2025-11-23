#pragma once

#include "BaseCardPage.h"

class QLineEdit;
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
        void setDescriptionText(const QString& text);

        QPushButton* getStartButton() const { return mStartBtn; }
        QPushButton* getReturnButton() const { return mReturnBtn; }
        bool isPaused() const { return mIsPaused; }
    signals:
        void startClicked();
        void pauseClicked();
        void stopClicked();
        void returnClicked();

    private slots:
        void onStartButtonClicked();
        void onPauseButtonClicked();
        void onStopButtonClicked();
        void onReturnButtonClicked();

        void onUiTick();

    private:
        void updateTimerLabel();
    private:
        QLabel* mTimerLabel;
        QLabel* mDateLabel;
        QLabel* mProjectLabel;
        QLabel* mTaskLabel;
        QLineEdit* mDescription;
        QPushButton* mStartBtn;
        QPushButton* mPauseBtn;
        QPushButton* mStopBtn;
        QPushButton* mReturnBtn;

        QTimer* mUiTimer;
        int mElapsedSeconds;
        bool mIsPaused;
    };
} // ui