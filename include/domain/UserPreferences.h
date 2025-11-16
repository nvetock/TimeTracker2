#pragma once
#include <QString>


namespace timetracker
{
    class UserPreferences
    {
    public:
        UserPreferences() =  default;
        ~UserPreferences() = default;


    private:
        // ToDo: Move seconds thresholds away from user-defined preferences or add admin-lock feature??
        // ToDo: Flag to by-pass Asana need and work Async / CSV only.
        QString mUserName;
        int mInactivityThresholdSeconds;
        int mReminderIntervalThresholdSeconds;
        int mHardStopThresholdSeconds;
        QString mCsvFilePath;
        QString mAsanaToken; //ToDo: This info likely needs to be kept secret.
        QString mAsanaPortfolioId;

        // ToDo: Load and save functions?
    };
}
