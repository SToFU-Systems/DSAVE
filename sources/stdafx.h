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

#include <windows.h>
#include <shlwapi.h>
#include <winnt.h>

#include "Tools.h"
#include "NTPEParser.h"
#include "FileInformation.h"
#include "ImportTools.h"

