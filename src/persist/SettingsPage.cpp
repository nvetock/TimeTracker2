#include "SettingsPage.h"

#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include "AppSettings.h"

namespace data
{
    SettingsPage::SettingsPage(AppSettings* settings, QWidget* parent)
        : QWidget{parent}
        , mSettings{settings}
        , mNameEdit{nullptr}
        , mDirEdit{nullptr}
        , mBrowseButton{nullptr}
        , mSaveButton{nullptr}
    {
        buildUi();
        loadFromSettings();
        mNameEdit = new QLineEdit(settings->getUserName());
        mDirEdit = new QLineEdit(settings->getSaveDir());
    }

    void SettingsPage::buildUi()
    {
        auto* rootLayout = new QVBoxLayout(this);
        rootLayout->setContentsMargins(24,24,24,24);
        rootLayout->setSpacing(16);
    }

    void SettingsPage::loadFromSettings()
    {
    }

    void SettingsPage::applyToSettings()
    {
    }

    /**
     * For opening file directory info
     */
    void SettingsPage::handleBrowse()
    {
    }

    /**
     * For saving the content
     */
    void SettingsPage::handleSave()
    {
    }
} // data