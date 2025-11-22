#include "infra/AppSettingsRepository.h"

#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

namespace infra
{
    AppSettingsRepository::AppSettingsRepository(QObject* parent)
        : QObject{parent}
    { }

    QString AppSettingsRepository::resolveBaseDir() const
    {
        const QString base = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

        const QDir dir{base};
        if (!dir.exists())
        {
            dir.mkpath(".");
        }
        return dir.absolutePath();
    }

    QString AppSettingsRepository::settingsFilePath() const
    {
        return resolveBaseDir() + QDir::separator() + "settings.json";
    }

    AppSettings AppSettingsRepository::load() const
    {
        AppSettings result;

        QFile file(settingsFilePath());
        if (!file.exists())
        {
            // The defaults when no settings exist yet
            result.userName = QString{};
            result.saveDirectory = resolveBaseDir();
            result.tasks = {};
            result.projects = {};
            return result;
        }

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qWarning() << "[AppSettingsRepository] load()\n"
                       << "  | Failed to open settings file for read.\n"
                       << file.errorString();
            return result;
        }

        const QByteArray data = file.readAll();
        file.close();

        QJsonParseError parseError;
        const QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
        if (parseError.error != QJsonParseError::NoError || !doc.isObject())
        {
            qWarning() << "[AppSettingsRepository] load()\n"
                       << "  | Invalid JSON:" << parseError.errorString();
            return result;
        }

        const QJsonObject root = doc.object();

        result.userName = root.value(QStringLiteral("userName")).toString();
        result.saveDirectory = root.value(QStringLiteral("saveDirectory")).toString();

        // Tasks
        const QJsonArray tasksArray = root.value(QStringLiteral("tasks")).toArray();
        for (const auto& v : tasksArray)
        {
            const QString s = v.toString().trimmed();
            if (!s.isEmpty()) result.tasks.append(s);
        }

        // Projects
        const QJsonArray projectsArray = root.value(QStringLiteral("projects")).toArray();
        for (const auto& v : projectsArray)
        {
            const QString s = v.toString().trimmed();
            if (!s.isEmpty()) result.projects.append(s);
        }

        // Fallbacks
        if (result.saveDirectory.isEmpty())
        {
            result.saveDirectory = resolveBaseDir();
        }

        return result;
    }

    bool AppSettingsRepository::save(const AppSettings& settings) const
    {
        QJsonObject root;
        root.insert(QStringLiteral("userName"), settings.userName);
        root.insert(QStringLiteral("saveDirectory"), settings.saveDirectory);

        QJsonArray tasksArray;
        for (const auto& t : settings.tasks)
        {
            const QString trimmed = t.trimmed();
            if (!trimmed.isEmpty()) tasksArray.append(trimmed);
        }
        root.insert(QStringLiteral("tasks"), tasksArray);

        QJsonArray projectsArray;
        for (const auto& p : settings.projects)
        {
            const QString trimmed = p.trimmed();
            if (!trimmed.isEmpty()) projectsArray.append(trimmed);
        }
        root.insert(QStringLiteral("projects"), projectsArray);

        const QJsonDocument doc{root};
        const QByteArray bytes = doc.toJson(QJsonDocument::Indented);

        QFile file(settingsFilePath());
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        {
            qWarning() << "[AppSettingsRepository] save()\n"
                       << "  | Failed to open settings file for write.\n"
                       << file.errorString();
            return false;
        }

        const qint64 written = file.write(bytes);
        file.close();

        if (written != bytes.size())
        {
            qWarning() << "[AppSettingsRepository] save()\n"
                       << "  | Incomplete write.";
            return false;
        }

        return true;
    }
} // infra