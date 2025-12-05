#include "Export2.h"

#include <QComboBox>
#include <QDate>
#include <qgridlayout.h>
#include <QRadioButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QLabel>

#include "InlineEditableLabel.h"
#include "UiElemStyler.h"

namespace ui
{
    Export2::Export2(QWidget* parent)
        : BaseCardPage{parent}
        , mCsvRadio{nullptr}
        , mJsonRadio{nullptr}
        , mAllButton{nullptr}
    {
        setTitle("TRACK WORK");
        showBackButton(true);

        auto* formatLabel = new QLabel("FORMAT", this);
        formatLabel->setObjectName("StatusLabel"); // reuse small label styling
        formatLabel->setAlignment(Qt::AlignCenter);

        mCsvRadio = generateButton("CSV", "RadioBtn", 44, true, this);
        mJsonRadio = generateButton("Json", "RadioBtn", 44, true, this);

        auto* radioBtnLayout = new QHBoxLayout(this);
        auto* radioBtnGroup = new QButtonGroup(radioBtnLayout);
        radioBtnGroup->setExclusive(true);
        radioBtnGroup->addButton(mCsvRadio);
        radioBtnGroup->addButton(mJsonRadio);
        qDebug() << "added radio btns to btngroup.";

        setZeroMarginAndSpaceBetween(radioBtnLayout, 8);
        radioBtnLayout->addWidget(mCsvRadio);
        radioBtnLayout->addWidget(mJsonRadio);
        qDebug() << "created radiobtnLayout buttons.";

        auto* radioSection = new QVBoxLayout();
        setZeroMarginAndSpaceBetween(radioSection, 8);
        radioSection->addWidget(formatLabel);
        radioSection->addLayout(radioBtnLayout);


        // EXPORT BUTTONS

        auto* Export = new QLabel("EXPORT", this);
        Export->setObjectName("StatusLabel"); // reuse small label styling
        Export->setAlignment(Qt::AlignCenter);

        mAllButton = generateButton("All Logs", "PrimaryBtn", 44, this);
        mSingleButton = generateButton("Single Date", "PrimaryBtn", 44, this);
        mRangeButton = generateButton("Date Range", "PrimaryBtn", 44, this);
        qDebug() << "created export buttons.";
        auto* exportBtnLayout = new QVBoxLayout(this);
        setZeroMarginAndSpaceBetween(exportBtnLayout, 16);
        exportBtnLayout->setAlignment(Qt::AlignCenter);
        exportBtnLayout->addWidget(Export);
        exportBtnLayout->addWidget(mAllButton, 0, Qt::AlignHCenter);
        exportBtnLayout->addWidget(mSingleButton, 0, Qt::AlignHCenter);
        exportBtnLayout->addWidget(mRangeButton, 0, Qt::AlignHCenter);

        // CONNECTIONS

        connect(mCsvRadio, &QRadioButton::toggled,
            this, &Export2::onCsvToggled);
        connect(mJsonRadio, &QRadioButton::toggled,
            this, &Export2::onJsonToggled);

        if (auto* headerBack = getBackButton())
        {
            connect(headerBack, &QPushButton::clicked,
                    this, &Export2::onBackClicked);
        }


        // Set to body
        auto* body = getBodyLayout();
        setZeroMarginAndSpaceBetween(body, 32);
        body->setAlignment(Qt::AlignCenter);
        body->addLayout(radioSection);
        body->addLayout(exportBtnLayout);

    }

    void Export2::onCsvToggled(const bool checked)
    {
        if (!checked) return;

        // makes this whole button selected
    }

    void Export2::onJsonToggled(const bool checked)
    {
        if (!checked) return;

        // makes this whole button selected
    }

    void Export2::onAllClicked()
    {
        // Handle export all logs
        return;
    }

    void Export2::onRangeClicked()
    {
        // Display calendar and look for from & to date set & submit
        return;
    }

    void Export2::onDayClicked()
    {
        // Display calendar and look for single day set & submit
        return;
    }

    void Export2::onBackClicked()
    {
        emit backRequested();
    }
} // ui