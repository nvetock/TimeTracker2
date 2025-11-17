#include "idle/MacIdleDetector.h"

#include <cstdint>
#include <qlogging.h>
#include <QDebug>

// #include <CoreFoundation/CoreFoundation.h>
// #include <IOKit/IOKitLib.h>

namespace timetracker
{
    /*
    // Returns HIDIdleTime in nanoseconds, or 0 on error.
    uint64_t getIdleTimeNanoseconds()
    {
        // Match the IOHIDSystem service
        CFMutableDictionaryRef dict = IOServiceMatching("IOHIDSystem");
        if (!dict)
        {
            qCritical() << "IOServiceMatching(IOHIDSystem) failed";
            return 0;
        }

        io_iterator_t iter = IO_OBJECT_NULL;
        kern_return_t kr = IOServiceGetMatchingServices(kIOMasterPortDefault,
                                                        dict,
                                                        &iter);
        if (kr != KERN_SUCCESS || iter == IO_OBJECT_NULL)
        {
            qCritical() << "IOServiceGetMatchingServices failed with code" << kr;
            return 0;
        }

        io_registry_entry_t entry = IOIteratorNext(iter);
        IOObjectRelease(iter);      // done with iterator

        if (entry == IO_OBJECT_NULL)
        {
            qCritical() << "No IOHIDSystem entry found";
            return 0;
        }

        // Read the HIDIdleTime property (nanoseconds)
        CFTypeRef value = IORegistryEntryCreateCFProperty(
            entry,
            CFSTR("HIDIdleTime"),
            kCFAllocatorDefault,
            0
        );

        IOObjectRelease(entry);     // done with registry entry

        if (!value)
        {
            qCritical() << "HIDIdleTime property not found";
            return 0;
        }

        uint64_t nanos = 0;

        if (CFGetTypeID(value) == CFNumberGetTypeID())
        {
            if (!CFNumberGetValue(static_cast<CFNumberRef>(value),
                             kCFNumberSInt64Type,
                             &nanos))
            {
                qCritical() << "CFNumberGetValue for HIDIdleTime failed.";
            }
        }
        else
        {
            qCritical() << "HIDIdleTime has unexpected CFTypeID.";
        }

        CFRelease(value);

        return nanos;
    }
*/

    int MacIdleDetector::idleSeconds() const
    {
        // const uint64_t nanos = getIdleTimeNanoseconds();
        // if (nanos == 0) return 0;
        // return static_cast<int>(nanos / 1'000'000'000ULL);
        return 0;
    }
}