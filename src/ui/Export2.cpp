#include "Export2.h"
#include "Export2.h"

#include <QComboBox>
#include <QDate>
#include <QRadioButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QDateEdit>
#include <QLabel>
#include <QCalendarWidget>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>

#include "InlineEditableLabel.h"
#include "UiElemStyler.h"

namespace ui
{
    Export2::Export2(QWidget* parent)
        : BaseCardPage{parent}
        , mExportFormat{ExportFormat::Csv}
        , mCsvRadio{nullptr}
        , mJsonRadio{nullptr}
        , mAllButton{nullptr}
        , mSingleButton{nullptr}
        , mRangeButton{nullptr}
        , mSettingsButton{nullptr}
        , mMenuButton{nullptr}
    {
        setTitle("TRACK WORK");
        showBackButton(true);

        auto* formatLabel = new QLabel("FORMAT", this);
        formatLabel->setObjectName("StatusLabel"); // reuse small label styling
        formatLabel->setAlignment(Qt::AlignCenter);

        mCsvRadio = generateButton("CSV", "RadioBtn", 44, true, this);
        mJsonRadio = generateButton("Json", "RadioBtn", 44, true, this);

        mCsvRadio->setChecked(true);
        auto* radioBtnLayout = new QHBoxLayout();
        auto* radioBtnGroup = new QButtonGroup(radioBtnLayout);
        radioBtnGroup->setExclusive(true);
        radioBtnGroup->addButton(mCsvRadio);
        radioBtnGroup->addButton(mJsonRadio);

        setZeroMarginAndSpaceBetween(radioBtnLayout, 8);
        radioBtnLayout->addWidget(mCsvRadio);
        radioBtnLayout->addWidget(mJsonRadio);

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

        auto* exportBtnLayout = new QVBoxLayout();
        setZeroMarginAndSpaceBetween(exportBtnLayout, 16);
        exportBtnLayout->setAlignment(Qt::AlignCenter);
        exportBtnLayout->addWidget(Export);
        exportBtnLayout->addWidget(mAllButton, 0, Qt::AlignHCenter);
        exportBtnLayout->addWidget(mSingleButton, 0, Qt::AlignHCenter);
        exportBtnLayout->addWidget(mRangeButton, 0, Qt::AlignHCenter);

        // FOOTER BUTTONS
        mMenuButton = generateButton("Menu", "SecondaryBtn", true, this);
        mSettingsButton = generateButton("Settings", "SecondaryBtn", true, this);
        auto* secondaryBtnGroup = new QVBoxLayout();
        secondaryBtnGroup->setSpacing(16);
        secondaryBtnGroup->addWidget(mSettingsButton,    0, Qt::AlignHCenter);
        secondaryBtnGroup->addWidget(mMenuButton,    0, Qt::AlignHCenter);



        // CONNECTIONS
        connect(mCsvRadio, &QRadioButton::toggled,
            this, [this]()
            {
                onFormatToggled(ExportFormat::Csv);
            });
        connect(mJsonRadio, &QRadioButton::toggled,
        this, [this]()
            {
                onFormatToggled(ExportFormat::Json);
            });
        connect(mMenuButton, &QPushButton::clicked,
        this, &Export2::onMenuClicked);
        connect(mAllButton, &QPushButton::clicked,
            this, &Export2::onAllClicked);
        connect(mSingleButton, &QPushButton::clicked,
            this, &Export2::onSingleClicked);
        connect(mRangeButton, &QPushButton::clicked,
            this, &Export2::onRangeClicked);


        // Set to body
        auto* body = getBodyLayout();
        setZeroMarginAndSpaceBetween(body, 32);
        body->setAlignment(Qt::AlignCenter);
        body->addLayout(radioSection);
        body->addLayout(exportBtnLayout);

        auto* footer = getFooterLayout();
        footer->addLayout(secondaryBtnGroup);
        footer->addSpacing(32);
    }

    QString Export2::exportFormatToString(const ExportFormat f)
    {
        switch (f)
        {
        case ExportFormat::Csv:
        default:
            return "csv";
        case ExportFormat::Json:
            return "json";
        }
    }

    void Export2::exportDates(const QDate& from, const QDate& to)
    {
        const QString f = exportFormatToString(mExportFormat);
        emit exportRequested(f, from, to);
    }

    void Export2::onFormatToggled(const ExportFormat type)
    {
        mExportFormat = type;
        qDebug() << "changing format to"
                 << exportFormatToString(mExportFormat);
    }

    void Export2::onAllClicked()
    {
        const QString format = exportFormatToString(mExportFormat);
        constexpr QDate from = QDate();
        constexpr QDate to = QDate();

        emit exportRequested(format, from, to);
    }

    void Export2::onRangeClicked()
    {
        // show calendar with an export button
    }

    void Export2::onSingleClicked()
    {
        // show calendar with an export button
        // Display calendar and look for single day set & submit
        QDialog dialog(this);
        dialog.setWindowTitle(tr("Export Single Date"));
        auto* layout = new QVBoxLayout(&dialog);
        auto* cal = new QCalendarWidget(&dialog);
        cal->setSelectedDate(QDate::currentDate());
        layout->addWidget(cal);

        auto* buttons = new QDialogButtonBox(
            QDialogButtonBox::Cancel | QDialogButtonBox::Ok, &dialog
        );
        buttons->button(QDialogButtonBox::Ok)->setText("Export");
        layout->addWidget(buttons);

        connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        if (dialog.exec() == QDialog::Accepted)
        {
            const QDate date = cal->selectedDate();
            const QString format = exportFormatToString(mExportFormat);
            emit exportRequested(format, date, date);
        }
    }

    void Export2::onMenuClicked()
    {
        emit menuRequested();
    }
} // ui