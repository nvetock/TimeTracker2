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
        explicit Export2(QWidget* parent = nullptr);

    signals:
        void backRequested();

        void exportRequested(const QString& format,
                             const QDate& fromDate,
                             const QDate& toDate);

    private slots:
        void onCsvToggled(bool checked);
        void onJsonToggled(bool checked);
        void onAllClicked();
        void onRangeClicked();
        void onDayClicked();

        void onBackClicked();

    private:
        QPushButton* mCsvRadio;
        QPushButton* mJsonRadio;
        QPushButton* mAllButton;
        QPushButton* mSingleButton;
        QPushButton* mRangeButton;
        QPushButton* mSettingsButton;
        QPushButton* mMenuButton;

    };
} // ui