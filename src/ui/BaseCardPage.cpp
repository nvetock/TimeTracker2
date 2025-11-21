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

#include "GradientFrame.h"

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
        , mBackChevron{nullptr}
        , mTitleLabel{nullptr}
        , mHeaderRule{nullptr}

        , mCardLayout{nullptr}
        , mBodyLayout{nullptr}
        , mFooterLayout{nullptr}
    {
        setAttribute(Qt::WA_TranslucentBackground);
        buildShell();
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
    mRootLayout->addWidget(mSideTab, 0, Qt::AlignTop);

    // --- Main card
    mMainCard = new GradientFrame{this};
    // If your QSS targets QFrame#MainCard, use this:
    mMainCard->setObjectName("MainCard");
    mMainCard->setFixedSize(360, 550);

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
    mHeaderLayout->setContentsMargins(0, 0, 0, 0);
    mHeaderLayout->setSpacing(4);

    // Title + Header row
    mTopHeaderRow = new QHBoxLayout;
    mTopHeaderRow->setContentsMargins(0, 0, 0, 0);
    mTopHeaderRow->setSpacing(0);

    mBackChevron = new QPushButton(QStringLiteral("<<"), mMainCard);
    mBackChevron->setObjectName("HeaderBackChevron");
    mBackChevron->setFlat(true);
    mBackChevron->setCursor(Qt::PointingHandCursor);
    mBackChevron->setFixedSize(24,24);

    mTitleLabel = new QLabel("UNTITLED", mMainCard);
    mTitleLabel->setObjectName("PageTitleLabel");
    mTitleLabel->setAlignment(Qt::AlignCenter);

    mHeaderRule = new QFrame(mMainCard);
    mHeaderRule->setObjectName("HeaderRule");
    mHeaderRule->setFrameShape(QFrame::HLine);
    mHeaderRule->setFrameShadow(QFrame::Plain);
    mHeaderRule->setLineWidth(1);

    mBackChevron->setVisible(false);

    mTopHeaderRow->addWidget(mBackChevron, 0, Qt::AlignLeft | Qt::AlignVCenter);
    mTopHeaderRow->addWidget(mTitleLabel);
    //mTopHeaderRow->addStretch();

    mHeaderLayout->addLayout(mTopHeaderRow);
    mHeaderLayout->addSpacing(4);
    mHeaderLayout->addWidget(mHeaderRule);
    mCardLayout->addLayout(mHeaderLayout);
    mCardLayout->setSpacing(12);

    QObject::connect(mBackChevron, &QPushButton::clicked,
                     this, &BaseCardPage::backRequested);

    // Body content
    mBodyLayout = new QVBoxLayout();
    mBodyLayout->setContentsMargins(0, 0, 0, 0);
    mBodyLayout->setSpacing(0);
    mCardLayout->addLayout(mBodyLayout);
    mCardLayout->addStretch(); // push footer to bottom

    // Footer
    mFooterLayout = new QVBoxLayout();
    mFooterLayout->setContentsMargins(0, 12, 0, 0);
    mFooterLayout->setSpacing(0);
    mCardLayout->addLayout(mFooterLayout);

    // Attach main card to ROOT
    mRootLayout->addWidget(mMainCard);
}

    void BaseCardPage::showBackButton(const bool show) const
    {
        if (mBackChevron) mBackChevron->setVisible(show);
    }

    void BaseCardPage::setTitle(const QString& title)
    {
        if (mTitleLabel)
        {
            mTitleLabel->setText(title);
        }
    }
} // ui