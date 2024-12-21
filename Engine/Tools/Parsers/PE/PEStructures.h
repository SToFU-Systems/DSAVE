/*
* 
* Based on <winnt.h>: Copyright (c) Microsoft Corporation. All rights reserved.
*
* All implied warranties, including merchantability and fitness, are disclaimed.
* Neither copyright owners nor contributors are liable for damages, including
* loss of data, profits, or business interruption, arising from use,
* even if warned of such possibilities.
*
*/

#ifndef __PE_STRUCTURES
#define __PE_STRUCTURES

#include "../../../Globals/BaseTypes.h"

// Disable structure alignment globally
#if defined(_MSC_VER)
#pragma pack(push, 1) // MSVC-specific directive
#elif defined(__GNUC__) || defined(__clang__)
#define PACKED __attribute__((packed))
#else
#error "Unsupported compiler. Add packing directives for your compiler."
#endif
#if defined(__GNUC__) || defined(__clang__)
PACKED
#endif

//***************************************************************************************************
// 
//                                      CONSTANTS
// 
//***************************************************************************************************


#define IMAGE_DOS_SIGNATURE                 0x5A4D      // MZ
#define IMAGE_OS2_SIGNATURE                 0x454E      // NE
#define IMAGE_OS2_SIGNATURE_LE              0x454C      // LE
#define IMAGE_VXD_SIGNATURE                 0x454C      // LE
#define IMAGE_NT_SIGNATURE                  0x00004550  // PE00

// Subsystem Values

#define IMAGE_SUBSYSTEM_UNKNOWN              0   // Unknown subsystem.
#define IMAGE_SUBSYSTEM_NATIVE               1   // Image doesn't require a subsystem.
#define IMAGE_SUBSYSTEM_WINDOWS_GUI          2   // Image runs in the Windows GUI subsystem.
#define IMAGE_SUBSYSTEM_WINDOWS_CUI          3   // Image runs in the Windows character subsystem.
#define IMAGE_SUBSYSTEM_OS2_CUI              5   // image runs in the OS/2 character subsystem.
#define IMAGE_SUBSYSTEM_POSIX_CUI            7   // image runs in the Posix character subsystem.
#define IMAGE_SUBSYSTEM_NATIVE_WINDOWS       8   // image is a native Win9x driver.
#define IMAGE_SUBSYSTEM_WINDOWS_CE_GUI       9   // Image runs in the Windows CE subsystem.
#define IMAGE_SUBSYSTEM_EFI_APPLICATION      10  //
#define IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER  11   //
#define IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER   12  //
#define IMAGE_SUBSYSTEM_EFI_ROM              13
#define IMAGE_SUBSYSTEM_XBOX                 14
#define IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION 16
#define IMAGE_SUBSYSTEM_XBOX_CODE_CATALOG    17

// DllCharacteristics Entries

//      IMAGE_LIBRARY_PROCESS_INIT            0x0001     // Reserved.
//      IMAGE_LIBRARY_PROCESS_TERM            0x0002     // Reserved.
//      IMAGE_LIBRARY_THREAD_INIT             0x0004     // Reserved.
//      IMAGE_LIBRARY_THREAD_TERM             0x0008     // Reserved.
#define IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA    0x0020  // Image can handle a high entropy 64-bit virtual address space.
#define IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE 0x0040     // DLL can move.
#define IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY    0x0080     // Code Integrity Image
#define IMAGE_DLLCHARACTERISTICS_NX_COMPAT    0x0100     // Image is NX compatible
#define IMAGE_DLLCHARACTERISTICS_NO_ISOLATION 0x0200     // Image understands isolation and doesn't want it
#define IMAGE_DLLCHARACTERISTICS_NO_SEH       0x0400     // Image does not use SEH.  No SE handler may reside in this image
#define IMAGE_DLLCHARACTERISTICS_NO_BIND      0x0800     // Do not bind this image.
#define IMAGE_DLLCHARACTERISTICS_APPCONTAINER 0x1000     // Image should execute in an AppContainer
#define IMAGE_DLLCHARACTERISTICS_WDM_DRIVER   0x2000     // Driver uses WDM model
#define IMAGE_DLLCHARACTERISTICS_GUARD_CF     0x4000     // Image supports Control Flow Guard.
#define IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE     0x8000

// Directory Entries

#define IMAGE_DIRECTORY_ENTRY_EXPORT          0   // Export Directory
#define IMAGE_DIRECTORY_ENTRY_IMPORT          1   // Import Directory
#define IMAGE_DIRECTORY_ENTRY_RESOURCE        2   // Resource Directory
#define IMAGE_DIRECTORY_ENTRY_EXCEPTION       3   // Exception Directory
#define IMAGE_DIRECTORY_ENTRY_SECURITY        4   // Security Directory
#define IMAGE_DIRECTORY_ENTRY_BASERELOC       5   // Base Relocation Table
#define IMAGE_DIRECTORY_ENTRY_DEBUG           6   // Debug Directory
//      IMAGE_DIRECTORY_ENTRY_COPYRIGHT       7   // (X86 usage)
#define IMAGE_DIRECTORY_ENTRY_ARCHITECTURE    7   // Architecture Specific Data
#define IMAGE_DIRECTORY_ENTRY_GLOBALPTR       8   // RVA of GP
#define IMAGE_DIRECTORY_ENTRY_TLS             9   // TLS Directory
#define IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG    10   // Load Configuration Directory
#define IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT   11   // Bound Import Directory in headers
#define IMAGE_DIRECTORY_ENTRY_IAT            12   // Import Address Table
#define IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT   13   // Delay Load Import Descriptors
#define IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR 14   // COM Runtime descriptor


#define IMAGE_SIZEOF_FILE_HEADER             20

#define IMAGE_FILE_RELOCS_STRIPPED           0x0001  // Relocation info stripped from file.
#define IMAGE_FILE_EXECUTABLE_IMAGE          0x0002  // File is executable  (i.e. no unresolved external references).
#define IMAGE_FILE_LINE_NUMS_STRIPPED        0x0004  // Line nunbers stripped from file.
#define IMAGE_FILE_LOCAL_SYMS_STRIPPED       0x0008  // Local symbols stripped from file.
#define IMAGE_FILE_AGGRESIVE_WS_TRIM         0x0010  // Aggressively trim working set
#define IMAGE_FILE_LARGE_ADDRESS_AWARE       0x0020  // App can handle >2gb addresses
#define IMAGE_FILE_BYTES_REVERSED_LO         0x0080  // Bytes of machine word are reversed.
#define IMAGE_FILE_32BIT_MACHINE             0x0100  // 32 bit word machine.
#define IMAGE_FILE_DEBUG_STRIPPED            0x0200  // Debugging info stripped from file in .DBG file
#define IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP   0x0400  // If Image is on removable media, copy and run from the swap file.
#define IMAGE_FILE_NET_RUN_FROM_SWAP         0x0800  // If Image is on Net, copy and run from the swap file.
#define IMAGE_FILE_SYSTEM                    0x1000  // System File.
#define IMAGE_FILE_DLL                       0x2000  // File is a DLL.
#define IMAGE_FILE_UP_SYSTEM_ONLY            0x4000  // File should only be run on a UP machine
#define IMAGE_FILE_BYTES_REVERSED_HI         0x8000  // Bytes of machine word are reversed.

#define IMAGE_FILE_MACHINE_UNKNOWN           0
#define IMAGE_FILE_MACHINE_TARGET_HOST       0x0001  // Useful for indicating we want to interact with the host and not a WoW guest.
#define IMAGE_FILE_MACHINE_I386              0x014c  // Intel 386.
#define IMAGE_FILE_MACHINE_R3000             0x0162  // MIPS little-endian, 0x160 big-endian
#define IMAGE_FILE_MACHINE_R4000             0x0166  // MIPS little-endian
#define IMAGE_FILE_MACHINE_R10000            0x0168  // MIPS little-endian
#define IMAGE_FILE_MACHINE_WCEMIPSV2         0x0169  // MIPS little-endian WCE v2
#define IMAGE_FILE_MACHINE_ALPHA             0x0184  // Alpha_AXP
#define IMAGE_FILE_MACHINE_SH3               0x01a2  // SH3 little-endian
#define IMAGE_FILE_MACHINE_SH3DSP            0x01a3
#define IMAGE_FILE_MACHINE_SH3E              0x01a4  // SH3E little-endian
#define IMAGE_FILE_MACHINE_SH4               0x01a6  // SH4 little-endian
#define IMAGE_FILE_MACHINE_SH5               0x01a8  // SH5
#define IMAGE_FILE_MACHINE_ARM               0x01c0  // ARM Little-Endian
#define IMAGE_FILE_MACHINE_THUMB             0x01c2  // ARM Thumb/Thumb-2 Little-Endian
#define IMAGE_FILE_MACHINE_ARMNT             0x01c4  // ARM Thumb-2 Little-Endian
#define IMAGE_FILE_MACHINE_AM33              0x01d3
#define IMAGE_FILE_MACHINE_POWERPC           0x01F0  // IBM PowerPC Little-Endian
#define IMAGE_FILE_MACHINE_POWERPCFP         0x01f1
#define IMAGE_FILE_MACHINE_IA64              0x0200  // Intel 64
#define IMAGE_FILE_MACHINE_MIPS16            0x0266  // MIPS
#define IMAGE_FILE_MACHINE_ALPHA64           0x0284  // ALPHA64
#define IMAGE_FILE_MACHINE_MIPSFPU           0x0366  // MIPS
#define IMAGE_FILE_MACHINE_MIPSFPU16         0x0466  // MIPS
#define IMAGE_FILE_MACHINE_AXP64             IMAGE_FILE_MACHINE_ALPHA64
#define IMAGE_FILE_MACHINE_TRICORE           0x0520  // Infineon
#define IMAGE_FILE_MACHINE_CEF               0x0CEF
#define IMAGE_FILE_MACHINE_EBC               0x0EBC  // EFI Byte Code
#define IMAGE_FILE_MACHINE_AMD64             0x8664  // AMD64 (K8)
#define IMAGE_FILE_MACHINE_M32R              0x9041  // M32R little-endian
#define IMAGE_FILE_MACHINE_ARM64             0xAA64  // ARM64 Little-Endian
#define IMAGE_FILE_MACHINE_CEE               0xC0EE

#define IMAGE_NT_OPTIONAL_HDR32_MAGIC      0x10b
#define IMAGE_NT_OPTIONAL_HDR64_MAGIC      0x20b
#define IMAGE_ROM_OPTIONAL_HDR_MAGIC       0x107


//***************************************************************************************************
// 
//                                      STRUCTURES
// 
//***************************************************************************************************

#include <stdint.h>

// DOS .EXE header
struct ImageDosHeader {
    uint16_t e_magic;        // Magic number
    uint16_t e_cblp;         // Bytes on last page of file
    uint16_t e_cp;           // Pages in file
    uint16_t e_crlc;         // Relocations
    uint16_t e_cparhdr;      // Size of header in paragraphs
    uint16_t e_minalloc;     // Minimum extra paragraphs needed
    uint16_t e_maxalloc;     // Maximum extra paragraphs needed
    uint16_t e_ss;           // Initial (relative) SS value
    uint16_t e_sp;           // Initial SP value
    uint16_t e_csum;         // Checksum
    uint16_t e_ip;           // Initial IP value
    uint16_t e_cs;           // Initial (relative) CS value
    uint16_t e_lfarlc;       // File address of relocation table
    uint16_t e_ovno;         // Overlay number
    uint16_t e_res[4];       // Reserved words
    uint16_t e_oemid;        // OEM identifier (for e_oeminfo)
    uint16_t e_oeminfo;      // OEM information; e_oemid specific
    uint16_t e_res2[10];     // Reserved words
    int32_t   e_lfanew;       // File address of new exe header
};

// IMAGE_FILE_HEADER structure
struct ImageFileHeader {
    uint16_t Machine;
    uint16_t NumberOfSections;
    uint32_t TimeDateStamp;
    uint32_t PointerToSymbolTable;
    uint32_t NumberOfSymbols;
    uint16_t SizeOfOptionalHeader;
    uint16_t Characteristics;
};

// IMAGE_DATA_DIRECTORY structure
struct ImageDataDirectory {
    uint32_t VirtualAddress;
    uint32_t Size;
};

// IMAGE_OPTIONAL_HEADER32 structure
struct ImageOptionalHeader32 {
    uint16_t Magic;                    // Identifies PE32 or PE32+ format
    uint8_t  MajorLinkerVersion;
    uint8_t  MinorLinkerVersion;
    uint32_t SizeOfCode;
    uint32_t SizeOfInitializedData;
    uint32_t SizeOfUninitializedData;
    uint32_t AddressOfEntryPoint;      // RVA of the entry point
    uint32_t BaseOfCode;
    uint32_t BaseOfData;
    uint32_t ImageBase;
    uint32_t SectionAlignment;
    uint32_t FileAlignment;
    uint16_t MajorOperatingSystemVersion;
    uint16_t MinorOperatingSystemVersion;
    uint16_t MajorImageVersion;
    uint16_t MinorImageVersion;
    uint16_t MajorSubsystemVersion;
    uint16_t MinorSubsystemVersion;
    uint32_t Win32VersionValue;
    uint32_t SizeOfImage;
    uint32_t SizeOfHeaders;
    uint32_t CheckSum;
    uint16_t Subsystem;
    uint16_t DllCharacteristics;
    uint32_t SizeOfStackReserve;
    uint32_t SizeOfStackCommit;
    uint32_t SizeOfHeapReserve;
    uint32_t SizeOfHeapCommit;
    uint32_t LoaderFlags;
    uint32_t NumberOfRvaAndSizes;
    struct ImageDataDirectory DataDirectory[16]; // Array of data directories
};

// IMAGE_OPTIONAL_HEADER64 structure
struct ImageOptionalHeader64 {
    uint16_t Magic;                    // Identifies PE32 or PE32+ format
    uint8_t  MajorLinkerVersion;
    uint8_t  MinorLinkerVersion;
    uint32_t SizeOfCode;
    uint32_t SizeOfInitializedData;
    uint32_t SizeOfUninitializedData;
    uint32_t AddressOfEntryPoint;      // RVA of the entry point
    uint32_t BaseOfCode;
    uint64_t ImageBase;
    uint32_t SectionAlignment;
    uint32_t FileAlignment;
    uint16_t MajorOperatingSystemVersion;
    uint16_t MinorOperatingSystemVersion;
    uint16_t MajorImageVersion;
    uint16_t MinorImageVersion;
    uint16_t MajorSubsystemVersion;
    uint16_t MinorSubsystemVersion;
    uint32_t Win32VersionValue;
    uint32_t SizeOfImage;
    uint32_t SizeOfHeaders;
    uint32_t CheckSum;
    uint16_t Subsystem;
    uint16_t DllCharacteristics;
    uint64_t SizeOfStackReserve;
    uint64_t SizeOfStackCommit;
    uint64_t SizeOfHeapReserve;
    uint64_t SizeOfHeapCommit;
    uint32_t LoaderFlags;
    uint32_t NumberOfRvaAndSizes;
    struct ImageDataDirectory DataDirectory[16]; // Array of data directories
};

// IMAGE_NT_HEADERS64 structure
struct ImageNtHeaders64 {
    uint32_t Signature;
    struct ImageFileHeader FileHeader;
    struct ImageOptionalHeader64 OptionalHeader;
};

// IMAGE_NT_HEADERS32 structure
struct ImageNtHeaders32 {
    uint32_t Signature; // 'PE\0\0' signature (0x00004550)
    struct ImageFileHeader FileHeader;
    struct ImageOptionalHeader32 OptionalHeader;
};

// IMAGE_NT_HEADERS structure
struct ImageNtHeaders {
    uint32_t Signature;
    struct ImageFileHeader FileHeader;
    struct ImageOptionalHeader32 OptionalHeader;
};

#define IMAGE_SIZEOF_SHORT_NAME 8

// IMAGE_SECTION_HEADER structure
struct ImageSectionHeader {
    uint8_t  Name[IMAGE_SIZEOF_SHORT_NAME];
    union {
        uint32_t PhysicalAddress;
        uint32_t VirtualSize;
    } Misc;
    uint32_t VirtualAddress;
    uint32_t SizeOfRawData;
    uint32_t PointerToRawData;
    uint32_t PointerToRelocations;
    uint32_t PointerToLinenumbers;
    uint16_t NumberOfRelocations;
    uint16_t NumberOfLinenumbers;
    uint32_t Characteristics;
};

// IMAGE_IMPORT_DESCRIPTOR structure
struct ImageImportDescriptor {
    union {
        uint32_t Characteristics;    // RVA to original unbound IAT
        uint32_t OriginalFirstThunk; // Same as Characteristics
    };
    uint32_t TimeDateStamp;          // Time/date stamp of the import
    uint32_t ForwarderChain;
    uint32_t Name;                   // RVA to the module name
    uint32_t FirstThunk;             // RVA to the IAT
};

// IMAGE_EXPORT_DIRECTORY structure
struct ImageExportDirectory {
    uint32_t Characteristics;
    uint32_t TimeDateStamp;
    uint16_t MajorVersion;
    uint16_t MinorVersion;
    uint32_t Name;                // RVA to the name of the DLL
    uint32_t Base;                // Starting ordinal number
    uint32_t NumberOfFunctions;   // Number of exported functions
    uint32_t NumberOfNames;       // Number of exported names
    uint32_t AddressOfFunctions;  // RVA to function addresses
    uint32_t AddressOfNames;      // RVA to names
    uint32_t AddressOfNameOrdinals; // RVA to ordinals
};

// IMAGE_IMPORT_BY_NAME structure
struct ImageImportByName {
    uint16_t Hint;          // Index into the export name table
    uint8_t  Name[1];       // Null-terminated string
};

// IMAGE_THUNK_DATA32 structure
struct ImageThunkData32 {
    union {
        uint32_t ForwarderString;  // RVA to forwarder string
        uint32_t Function;         // RVA to function
        uint32_t Ordinal;          // Ordinal value
        uint32_t AddressOfData;    // RVA to IMAGE_IMPORT_BY_NAME
    } u1;
};

// IMAGE_THUNK_DATA64 structure
struct ImageThunkData64 {
    union {
        uint64_t ForwarderString;  // RVA to forwarder string
        uint64_t Function;         // RVA to function
        uint64_t Ordinal;          // Ordinal value
        uint64_t AddressOfData;    // RVA to IMAGE_IMPORT_BY_NAME
    } u1;
};

// IMAGE_BASE_RELOCATION structure
struct ImageBaseRelocation {
    uint32_t VirtualAddress; // RVA of the base relocation block
    uint32_t SizeOfBlock;    // Size of the block, including entries
};




// Restore default structure alignment globally
#if defined(_MSC_VER)
#pragma pack(pop)
#elif defined(__GNUC__) || defined(__clang__)
#undef PACKED
#endif




//***************************************************************************************************
// 
//                                      MACROSES
// 
//***************************************************************************************************

#ifndef FIELD_OFFSET
#define FIELD_OFFSET(type, field)    ((uint64_t)(uint64_t)&(((type *)0)->field))
#define UFIELD_OFFSET(type, field)    ((uint64_t)(uint64_t)&(((type *)0)->field))
#endif

#define IMAGE_FIRST_SECTION( ntheader ) ((ImageSectionHeader*)        \
    ((uint8_t*)(ntheader) +                                            \
     FIELD_OFFSET( ImageNtHeaders, OptionalHeader ) +                 \
     ((ntheader))->FileHeader.SizeOfOptionalHeader   \
    ))





#endif