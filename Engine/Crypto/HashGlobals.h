#ifndef __HASHGLOBALS_H
#define __HASHGLOBALS_H

// HashGlobal.h
#include <algorithm>
#include <array>
#include <codecvt>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <locale>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

static const uint32_t hash_data_buf_size = 0x1000;
using hash_context  = std::array<uint8_t, 0x400 + hash_data_buf_size>;
using hash_data_buf = std::array<uint8_t, hash_data_buf_size>;
using hash_result   = std::array<uint8_t, 0x400>;

#endif

