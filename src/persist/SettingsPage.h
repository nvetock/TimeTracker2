#pragma once

#include <QWidget>

class QLineEdit;
class QPushButton;

namespace data
{
    class SettingsPage : public QWidget
    {
        Q_OBJECT

    public:
        SettingsPage(class AppSettings* settings, QWidget* parent = nullptr);

    signals:
        void settingsSaved();

    private slots:
        void handleBrowse();
        void handleSave();

    private:
        void buildUi();
        void loadFromSettings();
        void applyToSettings();

    private:
        AppSettings* mSettings;
        QLineEdit* mNameEdit;
        QLineEdit* mDirEdit;
        QPushButton* mBrowseButton;
        QPushButton* mSaveButton;
    };
} // data