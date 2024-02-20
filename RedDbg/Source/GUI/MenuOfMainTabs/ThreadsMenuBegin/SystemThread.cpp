#include "GUI/MenuOfMainTabs/ThreadsMenuBegin/SystemThread.hpp"

// Helper function to convert LARGE_INTEGER to milliseconds
void Thread::LargeIntegerToFileTime(const LARGE_INTEGER& li, FILETIME& ft) {
    ft.dwLowDateTime = li.LowPart;
    ft.dwHighDateTime = li.HighPart;
}

std::string Thread::ThreadFormatTime(const FILETIME& time) {
    // FILETIME is in  100ns intervals
    uint64_t time100ns = ((uint64_t)time.dwHighDateTime << 32) | (uint64_t)time.dwLowDateTime;
    uint64_t milliseconds = time100ns / 10000; // Convert to milliseconds

    // Calculate hours, minutes, seconds, and milliseconds
    milliseconds %= (1000 * 60 * 60 * 24);
    uint64_t hours = milliseconds / (1000 * 60 * 60);
    milliseconds %= (1000 * 60 * 60);
    uint64_t minutes = milliseconds / (1000 * 60);
    milliseconds %= (1000 * 60);
    uint64_t seconds = milliseconds / 1000;
    uint64_t remainingMilliseconds = milliseconds % 1000;

    SYSTEMTIME st;
    GetLocalTime(&st);

    long diff_hours = st.wHour - (hours % 24);
    if (diff_hours > 12) diff_hours -= 24;
    else if (diff_hours < -12) diff_hours += 24;

    if (hours > 0) { hours += diff_hours; }

    std::ostringstream oss;
    oss << std::setfill('0')
        << std::setw(2) << hours << ":"
        << std::setw(2) << minutes << ":"
        << std::setw(2) << seconds << "."
        << std::setw(7) << remainingMilliseconds;

    return oss.str();
}

void Thread::GetThreadsOfUserProcess(ThreadParserCallback callback) {
    NTSTATUS Status; ULONG size;
    if (NtQuerySystemInformation(SystemProcessInformation, NULL, 0, &size) != STATUS_INFO_LENGTH_MISMATCH) { return; }

    PSYSTEM_PROCESS_INFORMATION spi = new SYSTEM_PROCESS_INFORMATION[2 * size];
    RtlZeroMemory(spi, 2 * size);
    PSYSTEM_PROCESS_INFORMATION StartSpi = spi;
    if (!NT_SUCCESS(Status = NtQuerySystemInformation(SystemProcessInformation, spi, 2 * size, NULL)))
    {
        delete[] StartSpi;
        return;
    }

    while (true) // Loop over the list until we reach the last entry.
    {
        if (callback) {
            callback(spi);
        }
        if (spi->NextEntryOffset == 0) { break; }
        spi = PSYSTEM_PROCESS_INFORMATION((LPBYTE)spi + spi->NextEntryOffset);
    }
    delete[] StartSpi;

    return;
}