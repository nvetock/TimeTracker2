#include "MainMenuPage.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

#include "UiElemStyler.h"

namespace ui
{
    MainMenuPage::MainMenuPage(QWidget* parent)
        : BaseCardPage(parent)
    {
        setTitle("TIME TRACKER");

        auto* body   = getBodyLayout();

        // Minimal body – could be logo / tagline
        auto* titleLabel = generateLabel("What would you like to do?", "BodyCopy", "center", true, this);

        body->addSpacing(24);
        body->addWidget(titleLabel);
        body->addSpacing(32);

        mTrackWorkBtn = generateButton("Track Work", "PrimaryBtn", 44, this);
        mLogHistoryBtn = generateButton("Log History", "PrimaryBtn", 44, this);
        mExportBtn = generateButton("Export", "PrimaryBtn", 44, this);

        mSettingsBtn = generateButton("Settings", "SecondaryBtn", true, this);

        mQuitBtn = generateButton("Quit", "SecondaryBtn", true, this);

        auto* mainBtnGroup = new QVBoxLayout();
        setZeroMarginAndSpaceBetween(mainBtnGroup, 10);
        mainBtnGroup->addWidget(mTrackWorkBtn,   0, Qt::AlignHCenter);
        mainBtnGroup->addWidget(mLogHistoryBtn,  0, Qt::AlignHCenter);
        mainBtnGroup->addWidget(mExportBtn,      0, Qt::AlignHCenter);
        body->addLayout(mainBtnGroup);

        auto* secondaryBtnGroup = new QVBoxLayout();
        secondaryBtnGroup->setSpacing(4);
        secondaryBtnGroup->addWidget(mSettingsBtn,    0, Qt::AlignHCenter);
        secondaryBtnGroup->addWidget(mQuitBtn,        0, Qt::AlignHCenter);

        auto* footer = getFooterLayout();
        footer->addLayout(secondaryBtnGroup);
        footer->addSpacing(16);

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
