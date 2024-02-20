#pragma once
#include <functional>
#include <filesystem>
#include <string>
#include "Userntdll.h"

using ThreadParserCallback = std::function<void(PSYSTEM_PROCESS_INFORMATION)>;

namespace Thread {
	void GetThreadsOfUserProcess(ThreadParserCallback callback);
	void LargeIntegerToFileTime(const LARGE_INTEGER& li, FILETIME& ft);
	std::string ThreadFormatTime(const FILETIME& time);
}