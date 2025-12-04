#include "MainMenuPage.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

namespace ui
{
    MainMenuPage::MainMenuPage(QWidget* parent)
        : BaseCardPage(parent)
    {
        setTitle("TIME TRACKER");
        showBackButton(false); // root page – no back chevron

        auto* body   = getBodyLayout();
        auto* footer = getFooterLayout();

        // Minimal body – could be logo / tagline
        auto* titleLabel = new QLabel("What would you like to do?", this);
        titleLabel->setObjectName("BodyCopy");
        titleLabel->setAlignment(Qt::AlignCenter);

        body->addStretch();
        body->addSpacing(48);
        body->addWidget(titleLabel);
        body->addSpacing(64);

        // Footer: main actions
        mTrackWorkBtn = new QPushButton(tr("TRACK WORK"), this);
        mTrackWorkBtn->setObjectName("PrimaryBtn");
        mTrackWorkBtn->setFixedHeight(44);
        // mTrackWorkBtn->setFixedWidth(220);

        mLogHistoryBtn = new QPushButton(tr("LOG HISTORY"), this);
        mLogHistoryBtn->setObjectName("PrimaryBtn");
        mLogHistoryBtn->setFixedHeight(44);
        // mLogHistoryBtn->setFixedWidth(220);

        mExportBtn = new QPushButton(tr("EXPORT"), this);
        mExportBtn->setObjectName("PrimaryBtn");
        mExportBtn->setFixedHeight(44);
        // mExportBtn->setFixedWidth(220);

        mSettingsBtn = new QPushButton(tr("SETTINGS"), this);
        mSettingsBtn->setObjectName("SecondaryBtn");
        mSettingsBtn->setFixedHeight(44);
        // mSettingsBtn->setFixedWidth(220);
        mSettingsBtn->setFlat(true);

        mQuitBtn = new QPushButton(tr("QUIT"), this);
        mQuitBtn->setObjectName("SecondaryBtn");
        mQuitBtn->setFlat(true);

        auto* mainBtnGroup = new QVBoxLayout();
        mainBtnGroup->setContentsMargins(0, 0, 0, 0);
        mainBtnGroup->setSpacing(16);
        mainBtnGroup->addWidget(mTrackWorkBtn,   0, Qt::AlignHCenter);
        mainBtnGroup->addWidget(mLogHistoryBtn,  0, Qt::AlignHCenter);
        mainBtnGroup->addWidget(mExportBtn,      0, Qt::AlignHCenter);
        body->addLayout(mainBtnGroup);
        auto* secondaryBtnGroup = new QVBoxLayout();
        secondaryBtnGroup->setSpacing(8);
        secondaryBtnGroup->addWidget(mSettingsBtn,    0, Qt::AlignHCenter);
        secondaryBtnGroup->addWidget(mQuitBtn,        0, Qt::AlignHCenter);

        footer->addLayout(secondaryBtnGroup);
        footer->addSpacing(32);

        // Signals
        connect(mTrackWorkBtn,  &QPushButton::clicked,
                this,           &MainMenuPage::trackWorkRequested);

        connect(mLogHistoryBtn, &QPushButton::clicked,
                this,           &MainMenuPage::logHistoryRequested);

        connect(mExportBtn,     &QPushButton::clicked,
                this,           &MainMenuPage::exportRequested);

        connect(mSettingsBtn,   &QPushButton::clicked,
                this,           &MainMenuPage::settingsRequested);

        connect(mQuitBtn,       &QPushButton::clicked,
                this,           &MainMenuPage::quitRequested);
    }
}
