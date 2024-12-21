#pragma once

#include <vector>
#include <string>
#include "FileTypes.h"

namespace Files
{
	std::vector<uint8_t> readFullFile(const std::wstring& path);
}