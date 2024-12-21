cmake_minimum_required(VERSION 3.15)
project(avsdk LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(avsdk Engine/Crypto/FuzzyHashes/ssdeep/edit_dist.h Engine/Crypto/FuzzyHashes/ssdeep/ssdeep.h Engine/Crypto/FuzzyHashes/ssdeep/ssdeep_wrap.h Engine/Crypto/FuzzyHashes/ssdeep/sum_table.h Engine/Crypto/FuzzyHashes/tlsh/tlsh.h Engine/Crypto/FuzzyHashes/tlsh/tlsh_impl.h Engine/Crypto/FuzzyHashes/tlsh/tlsh_util.h Engine/Crypto/FuzzyHashes/tlsh/tlsh_version.h Engine/Crypto/FuzzyHashes/tlsh/tlsh_wrap.h Engine/Crypto/Hashes/crc32.h Engine/Crypto/Hashes/Hash.h Engine/Crypto/Hashes/HashGlobals.h Engine/Crypto/Hashes/md5.h Engine/Crypto/Hashes/sha1.h Engine/Crypto/Hashes/sha256.h Engine/Crypto/Hashes/sha512.h Engine/Globals/BaseTypes.h Engine/stdafx.h Engine/Tools/Containers/ThreadSafeVector.h Engine/Tools/Containers/Graph.h Engine/Tools/Files/FileTypes.h Engine/Tools/Parsers/PE/PEStructures.h Engine/Tools/System/FSParser.h Engine/Tools/System/Process.h Engine/Crypto/FuzzyHashes/ssdeep/edit_dist.cpp Engine/Crypto/FuzzyHashes/ssdeep/ssdeep.cpp Engine/Crypto/FuzzyHashes/ssdeep/ssdeep_wrap.cpp Engine/Crypto/FuzzyHashes/tlsh/tlsh.cpp Engine/Crypto/FuzzyHashes/tlsh/tlsh_impl.cpp Engine/Crypto/FuzzyHashes/tlsh/tlsh_util.cpp Engine/Crypto/FuzzyHashes/tlsh/tlsh_wrap.cpp Engine/Crypto/Hashes/crc32.cpp Engine/Crypto/Hashes/Hash.cpp Engine/Crypto/Hashes/md5.cpp Engine/Crypto/Hashes/sha1.cpp Engine/Crypto/Hashes/sha256.cpp Engine/Crypto/Hashes/sha512.cpp Engine/main.cpp Engine/Tools/Files/FileTypes.cpp Engine/Tools/System/FSParser.cpp Engine/Tools/System/Process.cpp)
target_include_directories(avsdk PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(avsdk PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/x64/Debug/avsdk"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/x64/Release/avsdk"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/x64/Debug/avsdk"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/x64/Release/avsdk"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/x64/Debug/avsdk"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/x64/Release/avsdk"
)

target_compile_definitions(avsdk PRIVATE UNICODE _UNICODE)

source_group("Source Files/Tools/Containers" FILES
    Engine/Tools/Containers/ThreadSafeVector.h
    Engine/Tools/Containers/Graph.h
)
source_group("Source Files/Tools/Files" FILES
    Engine/Tools/Files/FileTypes.h
    Engine/Tools/Files/FileTypes.cpp
)
source_group("Source Files/Tools/System" FILES
    Engine/Tools/System/FSParser.h
    Engine/Tools/System/Process.h
    Engine/Tools/System/FSParser.cpp
    Engine/Tools/System/Process.cpp
)
source_group("Source Files/Crypto/Hashes" FILES
    Engine/Crypto/Hashes/crc32.h
    Engine/Crypto/Hashes/Hash.h
    Engine/Crypto/Hashes/HashGlobals.h
    Engine/Crypto/Hashes/md5.h
    Engine/Crypto/Hashes/sha1.h
    Engine/Crypto/Hashes/sha256.h
    Engine/Crypto/Hashes/sha512.h
    Engine/Crypto/Hashes/crc32.cpp
    Engine/Crypto/Hashes/Hash.cpp
    Engine/Crypto/Hashes/md5.cpp
    Engine/Crypto/Hashes/sha1.cpp
    Engine/Crypto/Hashes/sha256.cpp
    Engine/Crypto/Hashes/sha512.cpp
)
source_group("Header Files" FILES
    Engine/stdafx.h
)
source_group("Source Files/Crypto/Fuzzy/Tlsh" FILES
    Engine/Crypto/FuzzyHashes/tlsh/tlsh.h
    Engine/Crypto/FuzzyHashes/tlsh/tlsh_impl.h
    Engine/Crypto/FuzzyHashes/tlsh/tlsh_util.h
    Engine/Crypto/FuzzyHashes/tlsh/tlsh_version.h
    Engine/Crypto/FuzzyHashes/tlsh/tlsh_wrap.h
    Engine/Crypto/FuzzyHashes/tlsh/tlsh.cpp
    Engine/Crypto/FuzzyHashes/tlsh/tlsh_impl.cpp
    Engine/Crypto/FuzzyHashes/tlsh/tlsh_util.cpp
    Engine/Crypto/FuzzyHashes/tlsh/tlsh_wrap.cpp
)
source_group("Source Files/Crypto/Fuzzy/Ssdeep" FILES
    Engine/Crypto/FuzzyHashes/ssdeep/edit_dist.h
    Engine/Crypto/FuzzyHashes/ssdeep/ssdeep.h
    Engine/Crypto/FuzzyHashes/ssdeep/ssdeep_wrap.h
    Engine/Crypto/FuzzyHashes/ssdeep/sum_table.h
    Engine/Crypto/FuzzyHashes/ssdeep/edit_dist.cpp
    Engine/Crypto/FuzzyHashes/ssdeep/ssdeep.cpp
    Engine/Crypto/FuzzyHashes/ssdeep/ssdeep_wrap.cpp
)
source_group("Source Files/Global" FILES
    Engine/Globals/BaseTypes.h
)
source_group("Source Files/Tools/Parsers/PE" FILES
    Engine/Tools/Parsers/PE/PEStructures.h
)
source_group("Source Files" FILES
    Engine/main.cpp
)

# Compiler and linker options based on platform and compiler
if(WIN32 AND MSVC)
    target_compile_options(avsdk PRIVATE /Gm- /EHsc /GS)
    target_link_options(avsdk PRIVATE /guard:cf /SUBSYSTEM:CONSOLE)

    target_compile_options(avsdk PRIVATE $<$<CONFIG:Debug>:/ZI /DWIN64 /D_WIN64>)
    target_link_options(avsdk PRIVATE $<$<CONFIG:Debug>:/MACHINE:X64>)

    target_compile_options(avsdk PRIVATE $<$<CONFIG:Release>:/Oy /O2 /Ob2 /Gy /GL /DWIN64 /D_WIN64>)
    target_link_options(avsdk PRIVATE $<$<CONFIG:Release>:/MACHINE:X64 /LTCG /OPT:REF /OPT:ICF /RELEASE>)
else()
    target_compile_options(avsdk PRIVATE
        -fexceptions
        -fstack-protector-all
        $<$<CONFIG:Debug>:-g -O0 -m64>
        $<$<CONFIG:Release>:-O3 -fomit-frame-pointer -m64 -fdata-sections -ffunction-sections -fno-plt -flto>
    )

    target_link_options(avsdk PRIVATE
        $<$<CONFIG:Debug>:-m64>
        $<$<CONFIG:Release>:-m64 -Wl,--gc-sections -Wl,--icf=all -Wl,--strip-all -Wl,--build-id=none -flto>
    )
endif()

