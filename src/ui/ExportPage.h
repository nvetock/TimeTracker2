#pragma once

#include "BaseCardPage.h"

class QRadioButton;
class QDateEdit;
class QDate;
class QComboBox;

namespace ui
{
    class ExportPage : public BaseCardPage
    {
        Q_OBJECT
    public:
        explicit ExportPage(QWidget* parent = nullptr);

        signals:
            void backRequested();

        // user pressed Export with these options
        void exportRequested(const QString& format,   // "csv" or "json"
                             const QDate& fromDate,
                             const QDate& toDate);

    private slots:
        void onAllToggled(bool checked);
        void onSingleDayToggled(bool checked);
        void onRangeToggled(bool checked);
        void onExportClicked();
        void onBackClicked();

    private:
        QRadioButton* mAllRadio;
        QRadioButton* mSingleDayRadio;
        QRadioButton* mRangeRadio;
        QDateEdit* mSingleDateEdit;
        QDateEdit* mFromDateEdit;
        QDateEdit* mToDateEdit;
        QComboBox* mFormatCombo; // CSV / JSON
        QPushButton* mExportBtn;
        QPushButton* mBackBtn;
    };

}
