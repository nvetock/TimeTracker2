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
        titleLabel->setObjectName("MainMenuPromptLabel");
        titleLabel->setAlignment(Qt::AlignCenter);

        body->addStretch();
        body->addWidget(titleLabel);
        body->addStretch();

        // Footer: main actions
        mTrackWorkBtn = new QPushButton(tr("TRACK WORK"), this);
        mTrackWorkBtn->setObjectName("PrimaryButton");
        mTrackWorkBtn->setFixedHeight(44);
        mTrackWorkBtn->setFixedWidth(220);

        mLogHistoryBtn = new QPushButton(tr("LOG HISTORY"), this);
        mLogHistoryBtn->setObjectName("SecondaryButton");
        mLogHistoryBtn->setFixedHeight(40);
        mLogHistoryBtn->setFixedWidth(220);

        mExportBtn = new QPushButton(tr("EXPORT"), this);
        mExportBtn->setObjectName("SecondaryButton");
        mExportBtn->setFixedHeight(40);
        mExportBtn->setFixedWidth(220);

        mSettingsBtn = new QPushButton(tr("SETTINGS"), this);
        mSettingsBtn->setObjectName("SecondaryButton");
        mSettingsBtn->setFixedHeight(40);
        mSettingsBtn->setFixedWidth(220);

        mQuitBtn = new QPushButton(tr("QUIT"), this);
        mQuitBtn->setObjectName("LinkButton");
        mQuitBtn->setFlat(true);

        auto* group = new QVBoxLayout();
        group->setContentsMargins(0, 0, 0, 0);
        group->setSpacing(8);
        group->addWidget(mTrackWorkBtn,   0, Qt::AlignHCenter);
        group->addWidget(mLogHistoryBtn,  0, Qt::AlignHCenter);
        group->addWidget(mExportBtn,      0, Qt::AlignHCenter);
        group->addWidget(mSettingsBtn,    0, Qt::AlignHCenter);
        group->addWidget(mQuitBtn,        0, Qt::AlignHCenter);

        footer->addLayout(group);

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
