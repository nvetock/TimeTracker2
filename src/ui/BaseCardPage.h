#pragma once

#include <QWidget>

#include "GradientFrame.h"

class QPushButton;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QGraphicsDropShadowEffect;

namespace ui
{
    class SideTab;

    class BaseCardPage : public QWidget
    {
        Q_OBJECT

    public:
        explicit BaseCardPage(QWidget* parent = nullptr);
        ~BaseCardPage() = default;

        void setRecordingActive(bool active);

        QVBoxLayout* getHeaderLayout() const { return mHeaderLayout; }
        QVBoxLayout* getBodyLayout() const { return mBodyLayout; }
        QVBoxLayout* getFooterLayout() const { return mFooterLayout; }

        void setTitle(const QString& title);
        QLabel* getTitleLabel() const { return mTitleLabel; }

        void showBackButton(bool show) const;
        QPushButton* getBackButton() const { return mBackChevron; }
    signals:
        void backRequested();



    private:
        void buildShell();

        QHBoxLayout* mRootLayout;
        SideTab* mSideTab;
        GradientFrame* mMainCard;
        QGraphicsDropShadowEffect* mShadow;

        // Header Items
        QHBoxLayout* mTopHeaderRow;
        QVBoxLayout* mHeaderLayout;
        QPushButton* mBackChevron;
        QLabel* mTitleLabel;
        QFrame* mHeaderRule;

        QVBoxLayout* mCardLayout;
        QVBoxLayout* mBodyLayout;
        QVBoxLayout* mFooterLayout;
    };
} // ui