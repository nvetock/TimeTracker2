#pragma once

namespace timetracker
{
    class IIdleDetector
    {
    public:
        IIdleDetector() = default;
        IIdleDetector(const IIdleDetector&) = delete;
        IIdleDetector& operator=(const IIdleDetector&) = delete;
        virtual ~IIdleDetector() = default;

        /**
         * Return the number of seconds since last user input.
         * @return int in seconds.
         */
        [[nodiscard]] virtual int idleSeconds() const = 0;
    };
}