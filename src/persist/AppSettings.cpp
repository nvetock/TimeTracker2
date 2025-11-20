#include "AppSettings.h"

#include <QSettings>
#include <QStandardPaths>
#include <QDir>

namespace data
{
    AppSettings::AppSettings(QObject* parent)
        : QObject{parent}
        , mUserName{""}
        , mSaveDir{""}
    {
        load();
    }

    void AppSettings::setUserName(const QString& userName)
    {
        if (mUserName == userName) return;  // already set
        mUserName = userName;
        emit settingsUpdated();
    }

    void AppSettings::setSaveDir(const QString& saveDir)
    {
        if (mSaveDir == saveDir) return;  // already set
        mSaveDir = saveDir;
        emit settingsUpdated();
    }

    void AppSettings::load()
    {
        QSettings s("Company", "TimeTracker");

        mUserName = s.value("userName").toString();
        mSaveDir = s.value("saveDir").toString();

        if (mSaveDir.isEmpty())
        {
            const QString docs = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
            mSaveDir = QDir(docs).filePath("TimeTracker");
        }
    }

    void AppSettings::save() const
    {
        QSettings s("Company", "TimeTracker");
        s.setValue("userName", mUserName);
        s.setValue("saveDir", mSaveDir);
    }
} // data