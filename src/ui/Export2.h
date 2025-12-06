#pragma once
#include "BaseCardPage.h"
#include "ClickableSvgWidget.h"

class QComboBox;
class QDate;
class QDateEdit;
class QRadioButton;
class QPushButton;

namespace ui
{
    class Export2 : public BaseCardPage
    {
        Q_OBJECT
    public:
        enum class ExportFormat { Csv, Json };

        explicit Export2(QWidget* parent = nullptr);

    signals:
        void menuRequested();

        void exportRequested(const QString& format,
                             const QDate& fromDate,
                             const QDate& toDate);

    private slots:
        void onFormatToggled(ExportFormat type);
        void onAllClicked();
        void onRangeClicked();
        void onSingleClicked();

        void onMenuClicked();

    private:
        QString exportFormatToString(const ExportFormat f);
        void exportDates(const QDate& from, const QDate& to);
    private:
        ExportFormat mExportFormat;
        QPushButton* mCsvRadio;
        QPushButton* mJsonRadio;
        QPushButton* mAllButton;
        QPushButton* mSingleButton;
        QPushButton* mRangeButton;
        QPushButton* mSettingsButton;
        QPushButton* mMenuButton;
    };
} // ui