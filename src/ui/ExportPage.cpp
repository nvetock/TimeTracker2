#include "ExportPage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>
#include <QDate>
#include "ClickableSvgWidget.h"

namespace ui
{
    ExportPage::ExportPage(QWidget* parent)
        : BaseCardPage{parent}
          , mAllRadio{nullptr}
          , mSingleDayRadio{nullptr}
          , mRangeRadio{nullptr}
          , mSingleDateEdit{nullptr}
          , mFromDateEdit{nullptr}
          , mToDateEdit{nullptr}
          , mFormatCombo{nullptr}
          , mExportBtn{nullptr}
          , mBackBtn{nullptr}
    {
        setTitle("EXPORT");
        showBackButton(true);


        // ---- Format row ----
        auto* formatLabel = new QLabel("FORMAT", this);
        formatLabel->setObjectName("StatusLabel"); // reuse small label styling
        formatLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        mFormatCombo = new QComboBox(this);
        mFormatCombo->addItem("CSV");
        mFormatCombo->addItem("JSON");

        auto* formatRow = new QHBoxLayout();
        formatRow->setContentsMargins(0, 0, 0, 0);
        formatRow->setSpacing(8);
        formatRow->addWidget(formatLabel);
        formatRow->addStretch();
        formatRow->addWidget(mFormatCombo);

        // ---- Range selection ----
        mAllRadio = new QRadioButton("All logs", this);
        mSingleDayRadio = new QRadioButton("Single day", this);
        mRangeRadio = new QRadioButton("Date range", this);

        mAllRadio->setChecked(true);

        const QDate today = QDate::currentDate();

        mSingleDateEdit = new QDateEdit(today, this);
        mSingleDateEdit->setDisplayFormat("yyyy-MM-dd");
        mSingleDateEdit->setCalendarPopup(true);

        mFromDateEdit = new QDateEdit(today, this);
        mFromDateEdit->setDisplayFormat("yyyy-MM-dd");
        mFromDateEdit->setCalendarPopup(true);

        mToDateEdit = new QDateEdit(today, this);
        mToDateEdit->setDisplayFormat("yyyy-MM-dd");
        mToDateEdit->setCalendarPopup(true);

        // Initially only "All logs" active
        mSingleDateEdit->setEnabled(false);
        mFromDateEdit->setEnabled(false);
        mToDateEdit->setEnabled(false);

        // Single-day row
        auto* singleRow = new QHBoxLayout();
        singleRow->setContentsMargins(0, 0, 0, 0);
        singleRow->setSpacing(8);
        singleRow->addWidget(mSingleDayRadio);
        singleRow->addWidget(mSingleDateEdit);
        singleRow->addStretch();

        // Range row
        auto* rangeRow = new QHBoxLayout();
        rangeRow->setContentsMargins(0, 0, 0, 0);
        rangeRow->setSpacing(8);
        rangeRow->addWidget(mRangeRadio);
        rangeRow->addWidget(new QLabel("From", this));
        rangeRow->addWidget(mFromDateEdit);
        rangeRow->addWidget(new QLabel("To", this));
        rangeRow->addWidget(mToDateEdit);
        rangeRow->addStretch();

        // Group all body content
        auto* bodyGroup = new QVBoxLayout();
        bodyGroup->setContentsMargins(0, 12, 0, 0);
        bodyGroup->setSpacing(8);
        bodyGroup->addLayout(formatRow);
        bodyGroup->addSpacing(16);
        bodyGroup->addWidget(mAllRadio);
        bodyGroup->addLayout(singleRow);
        bodyGroup->addLayout(rangeRow);

        auto* body = getBodyLayout();
        body->addLayout(bodyGroup);

        // ---- Footer buttons ----
        mExportBtn = new QPushButton("EXPORT", this);
        mExportBtn->setObjectName("PrimaryBtn");
        mExportBtn->setCursor(Qt::PointingHandCursor);
        mExportBtn->setFixedHeight(44);
        mExportBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        mBackBtn = new QPushButton("BACK", this);
        mBackBtn->setObjectName("SecondaryBtn");
        mBackBtn->setCursor(Qt::PointingHandCursor);
        mBackBtn->setFlat(true);
        mBackBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        auto* btnGroup = new QVBoxLayout();
        btnGroup->setContentsMargins(0, 0, 0, 0);
        btnGroup->setSpacing(10);
        btnGroup->addWidget(mExportBtn, 0, Qt::AlignHCenter);
        btnGroup->addWidget(mBackBtn, 0, Qt::AlignHCenter);

        auto* footer = getFooterLayout();
        footer->addLayout(btnGroup);
        footer->addSpacing(32);

        // ---- Connections ----
        connect(mAllRadio, &QRadioButton::toggled,
                this, &ExportPage::onAllToggled);
        connect(mSingleDayRadio, &QRadioButton::toggled,
                this, &ExportPage::onSingleDayToggled);
        connect(mRangeRadio, &QRadioButton::toggled,
                this, &ExportPage::onRangeToggled);

        connect(mExportBtn, &QPushButton::clicked,
                this, &ExportPage::onExportClicked);

        connect(mBackBtn, &QPushButton::clicked,
                this, &ExportPage::onBackClicked);

        // Also hook the header chevron to the same back behavior
        if (auto* headerBack = getBackButton())
        {
            connect(headerBack, &QPushButton::clicked,
                    this, &ExportPage::onBackClicked);
        }
    }

    void ExportPage::onAllToggled(bool checked)
    {
        if (!checked) return;

        mSingleDateEdit->setEnabled(false);
        mFromDateEdit->setEnabled(false);
        mToDateEdit->setEnabled(false);
    }

    void ExportPage::onRangeToggled(bool checked)
    {
        if (!checked) return;

        mSingleDateEdit->setEnabled(false);
        mFromDateEdit->setEnabled(true);
        mToDateEdit->setEnabled(true);
    }

    void ExportPage::onSingleDayToggled(bool checked)
    {
        if (!checked) return;

        mSingleDateEdit->setEnabled(true);
        mFromDateEdit->setEnabled(false);
        mToDateEdit->setEnabled(false);
    }

    void ExportPage::onExportClicked()
    {
        // "csv" / "json" (lowercase)
        const QString format = mFormatCombo->currentText().toLower();

        QDate from;
        QDate to;

        if (mAllRadio->isChecked())
        {
            // invalid dates => controller interprets as "all logs"
            from = QDate();
            to = QDate();
        }
        else if (mSingleDayRadio->isChecked())
        {
            from = mSingleDateEdit->date();
            to = from;
        }
        else // range
        {
            from = mFromDateEdit->date();
            to = mToDateEdit->date();
        }

        emit exportRequested(format, from, to);
    }

    void ExportPage::onBackClicked()
    {
        emit backRequested();
    }
}
