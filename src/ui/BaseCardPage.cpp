#include "BaseCardPage.h"
#include "SideTab.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QPushButton>

#include <QGuiApplication>
#include <QScreen>
#include <qstyle.h>

#include "GradientFrame.h"
#include "UiElemStyler.h"
#include "UiFlowController.h"

namespace ui
{
    BaseCardPage::BaseCardPage(QWidget* parent)
        : QWidget{parent}
        , mRootLayout{nullptr}
        , mSideTab{nullptr}
        , mMainCard{nullptr}
        , mShadow{nullptr}

        , mTopHeaderRow{nullptr}
        , mHeaderLayout{nullptr}
        , mTitleLabel{nullptr}
        , mHeaderRule{nullptr}

        , mCardLayout{nullptr}
        , mBodyLayout{nullptr}
        , mFooterLayout{nullptr}
    {
        setAttribute(Qt::WA_TranslucentBackground);
        buildShell();


    }

    void BaseCardPage::setRecordingActive(bool active)
    {
        if (!mSideTab) return;

        mSideTab->setProperty("recording", active);
        mSideTab->style()->unpolish(mSideTab);
        mSideTab->style()->polish(mSideTab);
        mSideTab->update();
    }

    void BaseCardPage::setIdlePaused(bool paused)
    {
        if (!mSideTab) return;

        mSideTab->setProperty("idle", paused);
        mSideTab->style()->unpolish(mSideTab);
        mSideTab->style()->polish(mSideTab);
        mSideTab->update();
    }

    void BaseCardPage::setEnded(bool ended)
    {
        if (!mSideTab) return;

        mSideTab->setProperty("ended", ended);
        mSideTab->style()->unpolish(mSideTab);
        mSideTab->style()->polish(mSideTab);
        mSideTab->update();
    }

    void BaseCardPage::buildShell()
{
    // ---- Root layout (holds [sidetab][maincard])
    mRootLayout = new QHBoxLayout{this};
    mRootLayout->setContentsMargins(0, 0, 0, 0);
    mRootLayout->setSpacing(0);

    // --- Side tab
    mSideTab = new SideTab{this};
    mSideTab->setObjectName("SideTab");
    mSideTab->setFixedSize(32, 164);
    mSideTab->setProperty("recording", false);
    mSideTab->setProperty("idle", false);
    mRootLayout->addWidget(mSideTab, 0, Qt::AlignTop);

    // --- Main card
    mMainCard = new GradientFrame{this};
    // If your QSS targets QFrame#MainCard, use this:
    mMainCard->setObjectName("MainCard");
    mMainCard->setFixedSize(360, 450);

    mCardLayout = new QVBoxLayout{mMainCard};
    mCardLayout->setContentsMargins(24, 20, 24, 20);
    mCardLayout->setSpacing(0);

    // Drop Shadow
    mShadow = new QGraphicsDropShadowEffect(mMainCard);
    mShadow->setBlurRadius(48);
    mShadow->setOffset(0, -12);
    mShadow->setColor(QColor(0, 0, 0, 100));
    mMainCard->setGraphicsEffect(mShadow);

    // --- Header layout (CREATE IT) ---------------------------------------
    mHeaderLayout = new QVBoxLayout();
    setZeroMarginAndSpaceBetween(mHeaderLayout, 4);

    // Title + Header row
    mTopHeaderRow = new QHBoxLayout;
    setZeroMarginAndSpaceBetween(mTopHeaderRow, 0);

    mTitleLabel = new QLabel("UNTITLED", mMainCard);
    mTitleLabel->setObjectName("PageTitleLabel");
    mTitleLabel->setAlignment(Qt::AlignCenter);

    mHeaderRule = new QFrame(mMainCard);
    mHeaderRule->setObjectName("HeaderRule");
    mHeaderRule->setFrameShape(QFrame::HLine);
    mHeaderRule->setFrameShadow(QFrame::Plain);
    mHeaderRule->setLineWidth(1);

    mTopHeaderRow->addWidget(mTitleLabel);
    //mTopHeaderRow->addStretch();

    mHeaderLayout->addLayout(mTopHeaderRow);
    mHeaderLayout->addSpacing(4);
    mHeaderLayout->addWidget(mHeaderRule);
    mCardLayout->addLayout(mHeaderLayout);
    mCardLayout->setSpacing(12);

    // Body content
    mBodyLayout = new QVBoxLayout();
    setZeroMarginAndSpaceBetween(mBodyLayout, 0);
    mCardLayout->addLayout(mBodyLayout);
    mCardLayout->addStretch(); // push footer to bottom

    // Footer
    mFooterLayout = new QVBoxLayout();
    setZeroMarginAndSpaceBetween(mFooterLayout, 0);
    mCardLayout->addLayout(mFooterLayout);

    // Attach main card to ROOT
    mRootLayout->addWidget(mMainCard);
}

void BaseCardPage::setTitle(const QString& title)
{
    if (mTitleLabel)
    {
        mTitleLabel->setText(title.toUpper());
    }
}

void BaseCardPage::onMenuClicked()
    {
        emit menuRequested();
    }
} // ui