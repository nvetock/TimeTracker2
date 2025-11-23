#pragma once

#include <QObject>
#include <QString>
#include <QVector>
#include <QTime>

namespace infra
{
    struct SessionLogEntry
    {
        QString userName;
        QString projectName;
        QString taskName;
        QString description;
        QString status;
        QDate date;
        QTime startTime;
        QTime endTime;
        int activeSeconds{0};
        int totalSeconds{0};
    };

    class SessionLogRepository : public QObject
    {
        Q_OBJECT

    public:
        explicit SessionLogRepository(const QString& logFilePath, QObject* parent = nullptr);

        // Append one entry to JSON file, return true on success.
        bool append(const SessionLogEntry& logEntry);

        // Load all entries
        QVector<SessionLogEntry> loadAll() const;

        QString filePath() const { return mFilePath; }

    private:
        QString mFilePath;
    };
} // infra