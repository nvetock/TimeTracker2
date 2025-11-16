#pragma once
#include <QString>


namespace timetracker
{
    class UserPreferences
    {
    public:

    private:
        // ToDo: Move seconds thresholds away from user-defined preferences or add admin-lock feature??
        // ToDo: Flag to by-pass Asana need and work Async / CSV only.
        int mInactivityThresholdSeconds;
        int mReminderIntervalThresholdSeconds;
        int mHardStopThresholdSeconds;
        QString mCsvFilePath;
        QString mAsanaToken;
        QString mAsanaPortfolioId;

        // ToDo: Load and save functions?
    };
}
