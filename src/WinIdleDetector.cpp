#include "idle/WinIdleDetector.h"

#include <Windows.h>
#include <QDebug>

namespace timetracker
{
    int WinIdleDetector::idleSeconds() const
    {
        LASTINPUTINFO lii{};
        lii.cbSize = sizeof(LASTINPUTINFO);

        if (!::GetLastInputInfo(&lii))
        {
            // Add a proper error logger here
            qCritical() << "Could not call GetLastInputInfo.";
            return 0;
        }

        // Tick count returns in milliseconds
        const ULONGLONG now = ::GetTickCount64();

        // If something weird occurs from the time difference return 0
        if (now < lii.dwTime) return 0;

        const ULONGLONG elapsedMs = now - lii.dwTime;

        return static_cast<int>(elapsedMs / 1000ULL);
    }
}
