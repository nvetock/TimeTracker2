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
#include <QTextCharFormat>

#include "InlineEditableLabel.h"
#include "UiElemStyler.h"

namespace ui
{
    Export2::Export2(QWidget* parent)
        : BaseCardPage{parent}
        , mExportFormat{ExportFormat::Csv}
        , mResultIcon{nullptr}
        , mResultLabel{nullptr}
        , mCsvRadio{nullptr}
        , mJsonRadio{nullptr}
        , mAllButton{nullptr}
        , mSingleButton{nullptr}
        , mRangeButton{nullptr}
        , mSettingsButton{nullptr}
        , mMenuButton{nullptr}
    {
        setTitle("TRACK WORK");

        auto* formatLabel = new QLabel("FORMAT", this);
        formatLabel->setObjectName("StatusLabel"); // reuse small label styling
        formatLabel->setAlignment(Qt::AlignCenter);

        mCsvRadio = generateButton("CSV", "RadioBtn", 32, true, this);
        mJsonRadio = generateButton("Json", "RadioBtn", 32, true, this);

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

        auto* Export = generateLabel("Export", "StatusLabel", "center", false, this);

            new QLabel("EXPORT", this);
        Export->setObjectName("StatusLabel"); // reuse small label styling
        Export->setAlignment(Qt::AlignCenter);

        mAllButton = generateButton("All Logs", "PrimaryBtn", 40, this);
        mSingleButton = generateButton("Single Date", "PrimaryBtn", 40, this);
        mRangeButton = generateButton("Date Range", "PrimaryBtn", 40, this);

        auto* exportBtnLayout = new QVBoxLayout();
        setZeroMarginAndSpaceBetween(exportBtnLayout, 8);
        exportBtnLayout->setAlignment(Qt::AlignCenter);
        exportBtnLayout->addWidget(Export);
        exportBtnLayout->addWidget(mAllButton, 0, Qt::AlignHCenter);
        exportBtnLayout->addWidget(mSingleButton, 0, Qt::AlignHCenter);
        exportBtnLayout->addWidget(mRangeButton, 0, Qt::AlignHCenter);


        // RESULT AREA
        mResultIcon = generateIcon(":/icons/resources/icons/check_circle.svg", this, "ExportResultIcon", 20);
        mResultIcon->hide();

        mResultLabel = new QLabel("", this);
        mResultLabel->setObjectName("ExportResultLabel");
        mResultLabel->setAlignment(Qt::AlignCenter);
        mResultLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        mResultLabel->hide();

        auto* resultLayout = new QHBoxLayout();
        setZeroMarginAndSpaceBetween(resultLayout, 4);
        resultLayout->setAlignment(Qt::AlignCenter);
        resultLayout->addWidget(mResultIcon, 0, Qt::AlignHCenter);
        resultLayout->addWidget(mResultLabel);


        // FOOTER BUTTONS
        mMenuButton = generateButton("Menu", "SecondaryBtn", true, this);
        mSettingsButton = generateButton("Settings", "SecondaryBtn", true, this);
        auto* secondaryBtnGroup = new QVBoxLayout();
        secondaryBtnGroup->setSpacing(4);
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
        connect(mSettingsButton, &QPushButton::clicked,
        this, &Export2::onSettingsClicked);
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
        setZeroMarginAndSpaceBetween(body, 16);
        body->setAlignment(Qt::AlignCenter);
        body->addSpacing(8);
        body->addLayout(radioSection);
        body->addLayout(exportBtnLayout);
        body->addLayout(resultLayout);

        auto* footer = getFooterLayout();
        footer->addLayout(secondaryBtnGroup);
        footer->addSpacing(16);
    }

    void Export2::showExportResult(bool success, const QString& message)
    {
        if (!mResultIcon || !mResultLabel) return;

        const QString iconPath = success
            ? QStringLiteral(":/icons/resources/icons/check_circle.svg")
            : QStringLiteral(":/icons/resources/icons/error.svg");

        mResultIcon->load(iconPath);

        mResultLabel->setProperty("exportSuccess", success ? "true" : "false");

        mResultLabel->style()->unpolish(mResultLabel);
        mResultLabel->style()->polish(mResultLabel);

        mResultLabel->setText(message);

        mResultIcon->show();
        mResultLabel->show();
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
        QDialog dialog(this);
        dialog.setWindowTitle(tr("Export Date Range"));

        auto* layout = new QVBoxLayout(&dialog);

        auto* cal = new QCalendarWidget(&dialog);
        cal->setSelectedDate(QDate::currentDate());
        layout->addWidget(cal);

        auto* dateLayout = new QHBoxLayout();
        setZeroMarginAndSpaceBetween(dateLayout, 4);
        dateLayout->setAlignment(Qt::AlignCenter);
        //auto* infoLayout = new QGridLayout();
        //infoLayout->setContentsMargins(0,0,0,0);
        //infoLayout->setHorizontalSpacing(8);
        //infoLayout->setVerticalSpacing(4);

        //auto* fromLabel = new QLabel(tr("From"), &dialog);
        auto* fromValue = new QLabel(tr("From"), &dialog);
        auto* separator = new QLabel(tr("->"), &dialog);
        auto* toValue = new QLabel(tr("To"), &dialog);
        //auto* toLabel = new QLabel(tr("To"), &dialog);

        dateLayout->addWidget(fromValue);
        dateLayout->addWidget(separator);
        dateLayout->addWidget(toValue);
        //infoLayout->addWidget(fromLabel, 0, 0);
        //infoLayout->addWidget(fromValue, 0, 1);
        //infoLayout->addWidget(toLabel, 1, 0);
        //infoLayout->addWidget(toValue, 1, 1);

        layout->addLayout(dateLayout);

        auto* buttons = new QDialogButtonBox(
            QDialogButtonBox::Cancel | QDialogButtonBox::Ok, &dialog
        );
        auto* confirmBtn = buttons->button(QDialogButtonBox::Ok);
        confirmBtn->setText("Export");
        confirmBtn->setEnabled(false);
        layout->addWidget(buttons);

        connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        QDate rangeStart;
        QDate rangeEnd;

        // Format highlighted area
        QTextCharFormat highlightFormat;
        highlightFormat.setBackground(QBrush(QColor(0, 224, 153, 60)));
        highlightFormat.setForeground(QBrush(Qt::white));

        QTextCharFormat edgeFormat = highlightFormat;
        edgeFormat.setFontWeight(QFont::Bold);

        // clear the previous highlight
        auto clearRangeFormatting = [cal](const QDate& from, const QDate& to)
        {
            if (!from.isValid() || !to.isValid()) return;

            QDate d = from;
            while (d <= to)
            {
                cal->setDateTextFormat(d, QTextCharFormat());
                d = d.addDays(1);
            }
        };

        // apply highlight to current range
        auto applyRangeFormatting = [&](const QDate& from, const QDate& to)
        {
            if (!from.isValid() || !to.isValid()) return;

            QDate d = from;
            while (d <= to)
            {
                if (d == from || d == to)
                {
                    cal->setDateTextFormat(d, edgeFormat);
                }
                else
                {
                    cal->setDateTextFormat(d, highlightFormat);
                }

                d = d.addDays(1);
            }
        };

        // CONNECTION on user clicking cal date
        QObject::connect(cal, &QCalendarWidget::clicked,
            &dialog, [&](const QDate& d)
            {
                // if no start is set OR a full range was previously selected
                // use this to start over
                if (!rangeStart.isValid() || (rangeStart.isValid() && rangeEnd.isValid()))
                {
                    // clear old
                    clearRangeFormatting(rangeStart, rangeEnd);

                    rangeStart = d;
                    rangeEnd = QDate();//invalid date

                    fromValue->setText(d.toString("yyyy-MM-dd"));
                    toValue->setText("-");
                    confirmBtn->setEnabled(false);

                    // highlight just start day
                    applyRangeFormatting(rangeStart, rangeStart);
                    return;
                }

                // Start is set, but no end yet -> finish range
                if (rangeStart.isValid() && !rangeEnd.isValid())
                {
                    clearRangeFormatting(rangeStart, rangeStart);

                    if (d < rangeStart)
                    {
                        rangeEnd = rangeStart;
                        rangeStart = d;
                    }
                    else
                    {
                        rangeEnd = d;
                    }

                    fromValue->setText(rangeStart.toString("yyyy-MM-dd"));
                    toValue->setText(rangeEnd.toString("yyyy-MM-dd"));

                    // apply highlight to full range
                    applyRangeFormatting(rangeStart, rangeEnd);
                    confirmBtn->setEnabled(true);
                }
            });

        if (dialog.exec() == QDialog::Accepted && rangeStart.isValid() && rangeEnd.isValid())
        {
            const QString format = exportFormatToString(mExportFormat);
            emit exportRequested(format, rangeStart, rangeEnd);
        }
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

    void Export2::onSettingsClicked()
    {
        emit settingsRequested();
    }
} // ui