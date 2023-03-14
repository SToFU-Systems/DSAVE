#pragma once

#define MAX_LONG_PATH_LEN 0x8000

#include <algorithm>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <map>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <ranges>

#include <windows.h>
#include <shlwapi.h>
#include <winnt.h>

#include "Math.h"
#include "Tools.h"
#include "NTPEParser.h"
#include "Import.h"

#include "FileInformation.h"


