#pragma once

#include <vector>
#include <string>
#include "FileTypes.h"

namespace Files
{
	uint64_t			 getFileSize(const std::wstring& path);
	std::vector<uint8_t> getFileData(const std::wstring& path);
}