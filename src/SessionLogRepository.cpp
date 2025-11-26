#include "infra/SessionLogRepository.h"

#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include <QDebug>
#include <QDate>
#include <algorithm>

namespace infra
{
    namespace json
    {
        QJsonObject toJson(const SessionLogEntry& entry)
        {
            QJsonObject obj{};
            obj["userName"] = entry.userName;
            obj["projectName"] = entry.projectName;
            obj["taskName"] = entry.taskName;
            obj["description"] = entry.description;
            obj["status"] = entry.status;
            obj["date"] = entry.date.toString("yyyy-MM-dd");
            obj["startTime"] = entry.startTime.toString("hh:mm:ss");
            obj["endTime"] = entry.endTime.toString("hh:mm:ss");
            obj["activeSeconds"] = entry.activeSeconds;
            obj["totalSeconds"] = entry.totalSeconds;

            return obj;
        }

        SessionLogEntry fromJson(const QJsonObject& obj)
        {
            SessionLogEntry entry{};
            entry.userName = obj.value("userName").toString();
            entry.projectName = obj.value("projectName").toString();
            entry.taskName = obj.value("taskName").toString();
            entry.description = obj.value("description").toString();
            entry.status = obj.value("status").toString();
            entry.date = QDate::fromString(obj.value("date").toString(), "yyyy-MM-dd");
            entry.startTime = QTime::fromString(obj.value("startTime").toString());
            entry.endTime = QTime::fromString(obj.value("endTime").toString());
            entry.activeSeconds = obj.value("activeSeconds").toInt();
            entry.totalSeconds = obj.value("totalSeconds").toInt();

            return entry;
        }
    }

    SessionLogRepository::SessionLogRepository(const QString& logFilePath, QObject* parent)
        : QObject{parent}
        , mFilePath{logFilePath}
    {
        // Make sure dir exists.
        QFile f{mFilePath};
        QDir dir = QFileInfo(f).dir();
        if (!dir.exists())
        {
            dir.mkpath(".");
        }

        // If this file doesn't exist, make empty structure
        if (!f.exists())
        {
            QJsonObject root;
            root["version"] = 1;
            root["entries"] = QJsonArray();

            if (f.open(QIODevice::WriteOnly | QIODevice::Truncate))
            {
                QJsonDocument doc{root};
                f.write(doc.toJson(QJsonDocument::Indented));
                f.close();
            }
            else
            {
                qWarning() << "[SessionLogRepository] ctor()\n"
                           << "  | Could not create log file:" << mFilePath;
            }
        }
    }

    bool SessionLogRepository::append(const SessionLogEntry& logEntry)
    {
        QFile f{mFilePath};
        if (!f.open(QIODevice::ReadOnly))
        {
            qWarning() << "[SessionLogRepository] append()\n"
                       << "  | Could not open file:" << mFilePath;
            return false;
        }

        const QByteArray data = f.readAll();
        f.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject root;

        if (doc.isObject()) root = doc.object();
        else if (doc.isNull())
        {
            root["version"] = 1;
            root["entries"] = QJsonArray();
        }
        else
        {
            qWarning() << "[SessionLogRepository] append()\n"
            << "  | Invalid JSON structure in file:" << mFilePath;
            return false;
        }

        QJsonArray entriesArray = root.value("entries").toArray();
        entriesArray.append(json::toJson(logEntry));
        root["entries"] = entriesArray;

        if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            qWarning() << "[SessionLogRepository] append()\n"
            << "  | Could not open file for write:" << mFilePath;
            return false;
        }

        doc.setObject(root);
        f.write(doc.toJson(QJsonDocument::Indented));
        f.close();

        qDebug() << "[SessionLogRepository] append()\n"
        << "  | Successfully created log file:" << mFilePath;

        return true;
    }

    QVector<SessionLogEntry> SessionLogRepository::loadAll() const
    {
        QVector<SessionLogEntry> out;

        QFile f{mFilePath};
        if (!f.open(QIODevice::ReadOnly))
        {
            qWarning() << "[SessionLogRepository] loadAll()\n"
                       << "  | Could not open file:" << mFilePath;
            return out;
        }

        const QByteArray data = f.readAll();
        f.close();

        const QJsonDocument doc = QJsonDocument::fromJson(data);
        if (!doc.isObject())
        {
            qWarning() << "[SessionLogRepository] loadAll()\n"
            << "  | Invalid JSON structure in file:" << mFilePath;
            return out;
        }

        const QJsonObject root = doc.object();
        const QJsonArray arr = root.value("entries").toArray();

        out.reserve(arr.size());
        for (const QJsonValue& v : arr)
        {
            if (!v.isObject()) continue;
            out.push_back(json::fromJson(v.toObject()));
        }

        return out;
    }


} // infra