#pragma once

#include <QObject>
#include <QString>
#include <QStringList>

namespace infra
{
    struct AppSettings
    {
        QString userName;
        QString saveDirectory;
        QStringList tasks;
        QStringList projects;
    };

    class AppSettingsRepository : public QObject
    {
        Q_OBJECT

    public:
        explicit AppSettingsRepository(QObject* parent = nullptr);

        // Load settings from JSON || Return defaults if none exist
        AppSettings load() const;

        // Save settings to JSON. Returns true on success
        bool save(const AppSettings& settings) const;

        // Path of the settings file.
        QString settingsFilePath() const;

    private:
        QString resolveBaseDir() const;
    };
} // infra