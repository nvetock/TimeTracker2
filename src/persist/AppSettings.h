#pragma once

#include <QObject>
#include <QString>

namespace data
{
    class AppSettings : public QObject
    {
        Q_OBJECT

    public:
        explicit AppSettings(QObject* parent = nullptr);

        QString getUserName() const { return mUserName; }
        void setUserName(const QString& userName);

        QString getSaveDir() const { return mSaveDir;}
        void setSaveDir(const QString& saveDir);

        void load();
        void save() const;

    signals:
        void settingsUpdated();

    private:
        QString mUserName;
        QString mSaveDir;
    };
} // data