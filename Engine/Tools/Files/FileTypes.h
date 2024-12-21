/*
* This Software is part of the SToFU Systems S.L. project and is licensed under the
* SToFU Non-Commercial License Agreement (Based on AGPL).
*
* Use, modification, and distribution are permitted exclusively by private individuals
* for personal, non-commercial purposes. Any use by corporations, organizations,
* institutions, or for any commercial or profit-oriented activity is strictly prohibited
* without prior written permission from SToFU Systems S.L.
*
* This Software may include third-party components under separate licenses. In such cases,
* their terms take precedence for those components.
*
* For full details, disclaimers, and commercial licensing inquiries, please refer to:
* https://stofu.io or the LICENSE file included with this Software.
*
* All implied warranties, including merchantability and fitness, are disclaimed.
* Neither copyright owners nor contributors are liable for damages, including
* loss of data, profits, or business interruption, arising from use,
* even if warned of such possibilities.
*
* Commercial Licensing Contact: contacts@stofu.io
*/

#ifndef __FILETYPES_H
#define __FILETYPES_H

#include <map>
#include <vector>

namespace Files
{

// Define your FileType enum with over 300 unique types, 16 per line for alignment
enum class FileType {
    // Unknown
    ext_unknown = 0,
    // Archives (1-16)
    ext_7z,          ext_bz2,         ext_gz,          ext_rar,          ext_tar,          ext_zip,           ext_iso,          ext_dmg,
    ext_msi,         ext_deb,         ext_rpm,         ext_lzma,         ext_xz,           ext_sitx,          ext_exe_archive,  ext_tgz,
    // Archives Continued (17-32)
    ext_tbz2,        ext_tlz,         ext_txz,         ext_z7,           ext_zstd,         ext_squashfs,      ext_xar,          ext_z,
    ext_gz2,         ext_bzip2,       ext_taz,         ext_cpio,         ext_arj,          ext_lha,           ext_lzh,          ext_zoo,
    // Audio (33-48)
    ext_flac,        ext_mp3,         ext_ogg,          ext_wav,          ext_aiff,          ext_wma,           ext_aac,          ext_ac3,
    ext_dts,         ext_tta,         ext_vorbis,       ext_spx,          ext_alac,          ext_m4a,           ext_ra,           ext_rm,
    ext_vag,         ext_ape,         ext_swa,          ext_afc,          ext_dra,           ext_eac3,          ext_caf,          ext_xspf,
    ext_zpl,         ext_pcas,        ext_xm,           ext_mod,          ext_it,            ext_s3m,           ext_fes,          ext_amf,
    ext_msm,         ext_upc,         ext_mtp,          ext_pcast,
    // Audio Continued (49-64)
    ext_aax,         ext_wv,          ext_mp2,          ext_mp1,          ext_mpa,           ext_cda,           ext_m3u,           ext_m3u8,
    ext_m4b,         ext_m4p,         ext_m4v,          ext_wvxl,         ext_dsfl,          ext_lpcm,          ext_sln,           ext_pcm,
    // Documents (65-80)
    ext_doc,         ext_docx,        ext_pdf,          ext_pptx,         ext_ps,            ext_txt_utf8,      ext_txt_utf16_be,  ext_txt_utf16_le,
    ext_xlsx,        ext_xml,         ext_odt,          ext_ods,          ext_odg,           ext_odc,           ext_ott,           ext_ots,
    ext_otg,         ext_otc,         ext_vsdx,         ext_pot,          ext_potx,          ext_ppam,          ext_ppsm,          ext_pptm,
    ext_potm,        ext_docm,        ext_dotx,         ext_dotm,         ext_xlsm,          ext_xltx,          ext_xltm,          ext_xlsb,
    ext_fods,        ext_fodt,        ext_fodp,         ext_fodg,         ext_fodc,          ext_foti,          ext_fotp,          ext_fotx,
    ext_fotm,
    // Images (97-112)
    ext_bmp,         ext_gif,         ext_jpeg,         ext_png,          ext_tiff_le,       ext_tiff_be,      ext_webp,          ext_svg,
    ext_heic,        ext_cr2,         ext_raw,          ext_tga,          ext_psb,           ext_fla,          ext_svgz,          ext_avif,
    ext_bpg,         ext_jxr,         ext_jpeg2000,     ext_exr,          ext_dng,           ext_nef,          ext_orf,           ext_rw2,
    ext_arw,         ext_srf,         ext_cr3,          ext_ciff,         ext_kdc,           ext_x3f,          ext_r3d,           ext_ept,
    ext_mxf,         ext_aaf,         ext_fpx,          ext_ras,          ext_ico,           ext_cur,          ext_dcm,           ext_gbr,
    ext_3fr,         ext_raf,         ext_k25,          ext_kdc_new,      ext_heif,          ext_webp_lossless,ext_jpegls,       
    // Executable Files (129-144)
    ext_elf,         ext_exe,         ext_dll,          ext_so,           ext_macho_32,      ext_macho_64,      ext_macho_fat,     ext_app,
    ext_bin,         ext_com,         ext_pif,          ext_cpl,          ext_sys,           ext_scr,           ext_mui,           ext_psp,
    // Executable Files Continued (145-160)
    ext_lnk,         ext_drv,         ext_kext,         ext_cab,          ext_dmg_exe,       ext_packed_exe,    ext_self_extract_exe, ext_shell_exe,
    ext_fat_exe,     ext_neko,        ext_node_exe,     ext_py_exe,       ext_java_exe,      ext_go_exe,        ext_rust_exe,         ext_dotnet_exe,
    // Certificates and Keys (161-176)
    ext_pem,         ext_pfx,         ext_cert,         ext_crt,          ext_key,           ext_p12,           ext_p7b,           ext_p7c,
    ext_p7m,         ext_p7r,         ext_p7s,          ext_p10,          ext_p8,            ext_pkcs12,        ext_pkcs7,         ext_p7a,
    // Fonts (177-192)
    ext_otf,         ext_ttf,         ext_woff,         ext_woff2,        ext_eot,           ext_sfp,           ext_pfb,           ext_afm,
    ext_fon,         ext_suit,        ext_bdf,          ext_pfa,          ext_pfb_otf,       ext_ttfa,          ext_ttc,
    // Videos (193-208)
    ext_avi,         ext_mpeg,        ext_webm,         ext_f4v,          ext_mkv,           ext_mov,           ext_mp4,           ext_flv,
    ext_vob,         ext_mpg,         ext_m2ts,         ext_ts,           ext_m2v,           ext_rmvb,          ext_svi,           ext_mpg4,
    // Videos Continued (209-224)
    ext_3gp,         ext_3g2,         ext_asf,          ext_wmv,          ext_dv,            ext_vrw,           ext_fli,           ext_flic,
    ext_swf,         ext_mng,         ext_hevc,         ext_h264,         ext_vp9,           ext_vp8,           ext_mpeg4,         ext_mp4a,
};

static const uint32_t maxMagicSequenceSize = 32;

typedef struct FileTypeRecord
{
    FileType fileType;
    uint8_t  numberOfNumbers;
    uint8_t  MagicNumbers[maxMagicSequenceSize];
};

typedef struct FileTypeArray
{
    FileTypeRecord records[300]; 
};

// Initialize the magicNumbers array with unique magic numbers
static const FileTypeArray fileTypeArray = {
    // Archives
    FileType::ext_7z,           6, {0x37, 0x7A, 0xBC, 0xAF, 0x27, 0x1C},                   // 7z Archive
    FileType::ext_bz2,          3, {0x42, 0x5A, 0x68},                                     // BZ2 Archive
    FileType::ext_gz,           2, {0x1F, 0x8B},                                           // GZ Archive
    FileType::ext_rar,          7, {0x52, 0x61, 0x72, 0x21, 0x1A, 0x07, 0x00},             // RAR Archive
    FileType::ext_tar,          5, {0x75, 0x73, 0x74, 0x61, 0x72},                         // TAR Archive (ustar)
    FileType::ext_zip,          4, {0x50, 0x4B, 0x03, 0x04},                               // ZIP Archive
    FileType::ext_iso,          5, {0x43, 0x44, 0x30, 0x30, 0x31},                         // ISO9660 CD Image
    FileType::ext_dmg,          9, {0x78, 0x01, 0x73, 0x74, 0x6F, 0x72, 0x61, 0x67, 0x65}, // Apple Disk Image
    FileType::ext_msi,          8, {0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1},       // MSI Installer
    FileType::ext_deb,          7, {0x21, 0x3C, 0x61, 0x72, 0x63, 0x68, 0x3E},             // Debian Package
    FileType::ext_rpm,          4, {0xED, 0xAB, 0xEE, 0xDB},                               // RPM Package
    FileType::ext_lzma,         5, {0x5D, 0x00, 0x00, 0x80, 0x00},                         // LZMA Archive
    FileType::ext_xz,           6, {0xFD, 0x37, 0x7A, 0x58, 0x5A, 0x00},                   // XZ Archive
    FileType::ext_sitx,         4, {0x53, 0x49, 0x54, 0x58},                               // StuffIt X Archive
    FileType::ext_tgz,          4, {0x1F, 0x8B, 0x08, 0x00},                               // TGZ Archive
    // Audio                                                                                 
    FileType::ext_flac,         4, {0x66, 0x4C, 0x61, 0x43},                               // FLAC Audio
    FileType::ext_mp3,          3, {0x49, 0x44, 0x33},                                     // MP3 Audio (ID3)
    FileType::ext_mp3,          2, {0xFF, 0xFB},                                           // MP3 Audio (MPEG)
    FileType::ext_ogg,          4, {0x4F, 0x67, 0x67, 0x53},                               // OGG Audio
    FileType::ext_wav,          4, {0x52, 0x49, 0x46, 0x46},                               // WAV Audio
    FileType::ext_aiff,         4, {0x46, 0x4F, 0x52, 0x4D},                               // AIFF Audio
    FileType::ext_wma,          16, {0x30, 0x26, 0xB2, 0x75, 0x8E, 0x66, 0xCF, 0x11,
                                      0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C},     // WMA Audio
    FileType::ext_aac,          2, {0xFF, 0xF1},                                           // AAC Audio
    FileType::ext_ac3,          4, {0x0B, 0x77, 0x68, 0x77},                               // AC3 Audio
    FileType::ext_dts,          4, {0x7F, 0xFE, 0x80, 0x01},                               // DTS Audio
    FileType::ext_tta,          3, {0x54, 0x54, 0x41},                                     // TTA Audio
    FileType::ext_vorbis,       4, {0x4F, 0x67, 0x67, 0x53},                               // Vorbis Audio
    FileType::ext_spx,          3, {0x53, 0x50, 0x58},                                     // Speex Audio
    FileType::ext_alac,         4, {0x4D, 0x4D, 0x4C, 0x41},                               // ALAC Audio
    FileType::ext_m4a,          8, {0x00, 0x00, 0x00, 0x14, 0x66, 0x74, 0x79, 0x70},       // M4A Audio
    FileType::ext_ra,           4, {0x2E, 0x52, 0x41, 0x48},                               // RealAudio
    FileType::ext_rm,           3, {0x2E, 0x52, 0x4D},                                     // RealMedia
    FileType::ext_aax,          4, {0x4D, 0x53, 0x41, 0x4E},                               // AAX Audio
    FileType::ext_wv,           4, {0x77, 0x56, 0x30, 0x30},                               // WavPack Audio
    FileType::ext_mp2,          2, {0xFF, 0xFB},                                           // MP2 Audio
    FileType::ext_mp1,          2, {0xFF, 0xF1},                                           // MP1 Audio
    FileType::ext_mpa,          2, {0xFF, 0xFB},                                           // MPA Audio
    FileType::ext_cda,          3, {0x43, 0x44, 0x41},                                     // CDA Audio
    FileType::ext_m3u,          5, {0x23, 0x21, 0x41, 0x49, 0x44},                         // M3U Playlist
    FileType::ext_m3u8,         5, {0x23, 0x21, 0x41, 0x49, 0x44},                         // M3U8 Playlist
    FileType::ext_m4v,          8, {0x00, 0x00, 0x00, 0x14, 0x66, 0x74, 0x79, 0x70},       // M4V Video (also used for audio)
    FileType::ext_wvxl,         4, {0x57, 0x56, 0x58, 0x4C},                               // WVXL Audio
    FileType::ext_dsfl,         4, {0x44, 0x53, 0x46, 0x4C},                               // DSF Audio
    FileType::ext_lpcm,         4, {0x4C, 0x50, 0x43, 0x4D},                               // LPCM Audio
    FileType::ext_sln,          4, {0x53, 0x4C, 0x4E, 0x20},                               // SLN Audio
    FileType::ext_pcm,          3, {0x50, 0x43, 0x4D},                                     // PCM Audio
    FileType::ext_vag,          4, {0x56, 0x41, 0x47, 0x21},                               // VAG Audio
    FileType::ext_ape,          4, {0x4D, 0x41, 0x43, 0x45},                               // APE Audio
    FileType::ext_swa,          3, {0x53, 0x57, 0x41},                                     // SWA Audio
    FileType::ext_afc,          3, {0x41, 0x46, 0x43},                                     // AFC Audio
    FileType::ext_dra,          3, {0x44, 0x52, 0x41},                                     // DRA Audio
    FileType::ext_eac3,         4, {0x0B, 0x77, 0x68, 0x77},                               // EAC3 Audio
    FileType::ext_caf,          4, {0x43, 0x41, 0x46, 0x66},                               // CAF Audio
    FileType::ext_xspf,         9, {0x3C, 0x3F, 0x78, 0x70, 0x61, 0x63, 0x6B, 0x65, 0x74}, // XSPF Playlist
    FileType::ext_zpl,          3, {0x5A, 0x50, 0x4C},                                     // ZPL Audio
    FileType::ext_pcast,        6, {0x2E, 0x50, 0x63, 0x61, 0x73, 0x74},                   // PCast Audio
    FileType::ext_xm,           4, {0x58, 0x4D, 0x50, 0x1A},                               // XM Audio
    FileType::ext_mod,          3, {0x4D, 0x4F, 0x44},                                     // MOD Audio
    FileType::ext_it,           4, {0x49, 0x54, 0x21, 0x1A},                               // IT Audio
    FileType::ext_s3m,          3, {0x53, 0x33, 0x4D},                                     // S3M Audio
    FileType::ext_fes,          4, {0x46, 0x45, 0x53, 0x54},                               // FES Audio
    FileType::ext_amf,          3, {0x41, 0x4D, 0x46},                                     // AMF Audio
    FileType::ext_msm,          3, {0x4D, 0x53, 0x4D},                                     // MSM Audio
    FileType::ext_upc,          3, {0x55, 0x50, 0x43},                                     // UPC Audio
    FileType::ext_mtp,          3, {0x4D, 0x54, 0x50},                                     // MTP Audio
    // Documents
    FileType::ext_doc,          8, {0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1},       // DOC (Old Binary)
    FileType::ext_docx,         4, {0x50, 0x4B, 0x03, 0x04},                               // DOCX (Office Open XML)
    FileType::ext_pdf,          4, {0x25, 0x50, 0x44, 0x46},                               // PDF Document
    FileType::ext_pptx,         4, {0x50, 0x4B, 0x03, 0x04},                               // PPTX (Office Open XML)
    FileType::ext_ps,           4, {0x25, 0x21, 0x50, 0x53},                               // PS (PostScript)
    FileType::ext_txt_utf8,     3, {0xEF, 0xBB, 0xBF},                                     // TXT (UTF-8 BOM)
    FileType::ext_txt_utf16_be, 2, {0xFE, 0xFF},                                           // TXT (UTF-16 BE BOM)
    FileType::ext_txt_utf16_le, 2, {0xFF, 0xFE},                                           // TXT (UTF-16 LE BOM)
    FileType::ext_xlsx,         4, {0x50, 0x4B, 0x03, 0x04},                               // XLSX (Office Open XML)
    FileType::ext_xml,          5, {0x3C, 0x3F, 0x78, 0x6D, 0x6C},                         // XML Document
    FileType::ext_odt,          4, {0x50, 0x4B, 0x03, 0x04},                               // ODT (OpenDocument Text)
    FileType::ext_fods,         4, {0x46, 0x4F, 0x44, 0x53},                               // FODS (Flat OpenDocument Spreadsheet)
    FileType::ext_fodt,         4, {0x46, 0x4F, 0x44, 0x54},                               // FODT (Flat OpenDocument Text)
    FileType::ext_fodp,         4, {0x46, 0x4F, 0x44, 0x50},                               // FODP (Flat OpenDocument Presentation)
    FileType::ext_fodg,         4, {0x46, 0x4F, 0x44, 0x47},                               // FODG (Flat OpenDocument Graphics)
    FileType::ext_fodc,         4, {0x46, 0x4F, 0x44, 0x43},                               // FODC (Flat OpenDocument Chart)
    FileType::ext_foti,         4, {0x46, 0x4F, 0x44, 0x49},                               // FOTI (Flat OpenDocument Image)
    FileType::ext_fotp,         4, {0x46, 0x4F, 0x44, 0x50},                               // FOTP (Flat OpenDocument Presentation Template)
    FileType::ext_fotx,         4, {0x46, 0x4F, 0x44, 0x58},                               // FOTX (Flat OpenDocument Text Template)
    FileType::ext_fotm,         4, {0x46, 0x4F, 0x44, 0x4D},                               // FOTM (Flat OpenDocument Text Macro-Enabled Template)
    // Images Continued
    FileType::ext_bmp,          2, {0x42, 0x4D},                                           // BMP Image
    FileType::ext_gif,          4, {0x47, 0x49, 0x46, 0x38},                               // GIF Image
    FileType::ext_jpeg,         3, {0xFF, 0xD8, 0xFF},                                     // JPEG Image
    FileType::ext_png,          4, {0x89, 0x50, 0x4E, 0x47},                               // PNG Image
    FileType::ext_tiff_le,      4, {0x49, 0x49, 0x2A, 0x00},                               // TIFF Image (Little Endian)
    FileType::ext_tiff_be,      4, {0x4D, 0x4D, 0x00, 0x2A},                               // TIFF Image (Big Endian)
    FileType::ext_webp,         4, {0x52, 0x49, 0x46, 0x46},                               // WEBP Image
    FileType::ext_svg,          4, {0x3C, 0x73, 0x76, 0x67},                               // SVG Image
    FileType::ext_heic,         4, {0x66, 0x74, 0x79, 0x70},                               // HEIC Image
    FileType::ext_cr2,          4, {0x43, 0x52, 0x32, 0x34},                               // Canon CR2 Image
    FileType::ext_raw,          4, {0x49, 0x49, 0x2A, 0x00},                               // RAW Image
    FileType::ext_tga,          3, {0x00, 0x00, 0x02},                                     // TGA Image
    FileType::ext_psb,          4, {0x38, 0x42, 0x50, 0x53},                               // PSB Image
    FileType::ext_fla,          3, {0x46, 0x4C, 0x41},                                     // FLA Image
    FileType::ext_svgz,         3, {0x1F, 0x8B, 0x08},                                     // SVGZ Image
    FileType::ext_avif,         12, {0x00, 0x00, 0x00, 0x18, 0x66, 0x74, 0x79, 0x70, 0x61, 0x76, 0x69, 0x66}, // AVIF Image
    FileType::ext_bpg,          4, {0x42, 0x50, 0x47, 0xFB},                               // BPG Image
    FileType::ext_jxr,          4, {0x49, 0x20, 0x4A, 0x58},                               // JXR Image
    FileType::ext_jpeg2000,     12, {0x00, 0x00, 0x00, 0x0C, 0x6A, 0x50, 0x20, 0x20, 0x0D, 0x0A, 0x87, 0x0A}, // JPEG2000 Image
    FileType::ext_exr,          4, {0x76, 0x2F, 0x31, 0x01},                               // EXR Image
    FileType::ext_dng,          4, {0x49, 0x49, 0x2A, 0x00},                               // DNG Image
    FileType::ext_nef,          4, {0x4E, 0x45, 0x46, 0x20},                               // NEF Image (Nikon)
    FileType::ext_orf,          4, {0x4F, 0x52, 0x46, 0x20},                               // ORF Image (Olympus)
    FileType::ext_rw2,          4, {0x52, 0x57, 0x32, 0x00},                               // RW2 Image (Panasonic)
    FileType::ext_arw,          4, {0x41, 0x52, 0x57, 0x00},                               // ARW Image (Sony)
    FileType::ext_srf,          4, {0x53, 0x52, 0x46, 0x20},                               // SRF Image (Sony)
    FileType::ext_cr3,          4, {0x43, 0x52, 0x33, 0x00},                               // CR3 Image (Canon)
    FileType::ext_ciff,         4, {0x43, 0x49, 0x46, 0x46},                               // CIFF Image (Phase One)
    FileType::ext_kdc,          4, {0x4B, 0x44, 0x43, 0x20},                               // KDC Image (Kodak)
    FileType::ext_x3f,          4, {0x58, 0x33, 0x46, 0x20},                               // X3F Image (Sigma)
    FileType::ext_r3d,          4, {0x52, 0x33, 0x44, 0x20},                               // R3D Image (RED)
    FileType::ext_ept,          3, {0x45, 0x50, 0x54},                                     // EPT Image
    FileType::ext_mxf,          6, {0x06, 0x0E, 0x2B, 0x34, 0x02, 0x05},                   // MXF Image
    FileType::ext_aaf,          4, {0x41, 0x41, 0x46, 0x20},                               // AAF Image
    FileType::ext_fpx,          3, {0x46, 0x50, 0x58},                                     // FPX Image
    FileType::ext_ras,          4, {0x59, 0xA6, 0x6A, 0x95},                               // RAS Image
    FileType::ext_ico,          4, {0x00, 0x00, 0x01, 0x00},                               // ICO Image
    FileType::ext_cur,          4, {0x00, 0x00, 0x02, 0x00},                               // CUR Image (Cursor)
    FileType::ext_dcm,          4, {0x44, 0x49, 0x43, 0x4D},                               // DCM Image (DICOM)
    FileType::ext_gbr,          3, {0x47, 0x42, 0x52},                                     // GBR Image
    FileType::ext_3fr,          3, {0x33, 0x46, 0x52},                                     // 3FR Image
    FileType::ext_raf,          4, {0x52, 0x41, 0x46, 0x41},                               // RAF Image (Fuji)
    FileType::ext_k25,          3, {0x4B, 0x32, 0x35},                                     // K25 Image
    FileType::ext_kdc_new,      4, {0x4B, 0x44, 0x43, 0x4E},                               // KDC New Image
    FileType::ext_heif,         8, {0x66, 0x74, 0x79, 0x70, 0x68, 0x65, 0x69, 0x66},       // HEIF Image
    FileType::ext_webp_lossless,4, {0x52, 0x49, 0x46, 0x46},                               // WEBP Lossless Image
    FileType::ext_jpegls,       3, {0xFF, 0x4C, 0x53},                                     // JPEG-LS Image
    // Executable Files
    FileType::ext_elf,          4, {0x7F, 0x45, 0x4C, 0x46},                               // ELF Executable
    FileType::ext_so,           4, {0x7F, 0x45, 0x4C, 0x46},                               // SO Executable (ELF)
    FileType::ext_macho_32,     4, {0xFE, 0xED, 0xFA, 0xCE},                               // Mach-O Executable (32-bit)
    FileType::ext_macho_64,     4, {0xFE, 0xED, 0xFA, 0xCF},                               // Mach-O Executable (64-bit)
    FileType::ext_macho_fat,    4, {0xCA, 0xFE, 0xBA, 0xBE},                               // Mach-O Executable (Fat Binary)
    FileType::ext_app,          2, {0x4D, 0x5A},                                           // APP Executable (macOS)
    FileType::ext_bin,          3, {0x42, 0x5A, 0x68},                                     // BIN Executable
    FileType::ext_com,          3, {0x43, 0x4F, 0x4D},                                     // COM Executable
    FileType::ext_psp,          3, {0x50, 0x53, 0x50},                                     // PSP Executable
    FileType::ext_lnk,          4, {0x4C, 0x00, 0x00, 0x00},                               // LNK Executable (Windows Shortcut)
    FileType::ext_drv,          4, {0x44, 0x52, 0x56, 0x00},                               // DRV Executable (Driver)
    FileType::ext_kext,         4, {0x4B, 0x45, 0x58, 0x54},                               // KEXT Executable (macOS Kernel Extension)
    FileType::ext_cab,          4, {0x4D, 0x53, 0x43, 0x46},                               // CAB Executable (Windows Cabinet)
    // Certificates and Keys
    FileType::ext_pem,          9, {0x2D, 0x2D, 0x2D, 0x2D, 0x42, 0x45, 0x47, 0x49, 0x4E}, // PEM Certificate
    FileType::ext_pfx,          2, {0x30, 0x82},                                           // PFX Certificate
    FileType::ext_cert,         2, {0x30, 0x82},                                           // CERT Certificate
    FileType::ext_crt,          2, {0x30, 0x82},                                           // CRT Certificate
    FileType::ext_key,          23, {0x2D, 0x2D, 0x2D, 0x2D, 0x45, 0x4E, 0x44, 0x20,
                                      0x50, 0x52, 0x49, 0x56, 0x41, 0x54, 0x45, 0x20,
                                      0x4B, 0x45, 0x59, 0x2D, 0x2D, 0x2D, 0x2D},           // KEY File
    FileType::ext_p12,          2, {0x30, 0x82},                                           // P12 Certificate

    // Fonts
    FileType::ext_otf,          4, {0x4F, 0x54, 0x54, 0x4F},                               // OTF Font
    FileType::ext_ttf,          4, {0x00, 0x01, 0x00, 0x00},                               // TTF Font
    FileType::ext_woff,         4, {0x77, 0x4F, 0x46, 0x46},                               // WOFF Font
    FileType::ext_woff2,        4, {0x77, 0x4F, 0x46, 0x32},                               // WOFF2 Font
    FileType::ext_eot,          4, {0x4C, 0x50, 0x43, 0x46},                               // EOT Font
    FileType::ext_sfp,          3, {0x53, 0x46, 0x50},                                     // SFP Font
    FileType::ext_pfb,          4, {0x80, 0x01, 0x00, 0x00},                               // PFB Font
    FileType::ext_afm,          4, {0x41, 0x46, 0x4D, 0x20},                               // AFM Font
    FileType::ext_fon,          4, {0x00, 0x00, 0x01, 0x00},                               // FON Font
    FileType::ext_suit,         4, {0x53, 0x55, 0x49, 0x54},                               // SUIT Font
    FileType::ext_bdf,          3, {0x42, 0x44, 0x46},                                     // BDF Font
    FileType::ext_pfa,          4, {0x01, 0x00, 0x01, 0x00},                               // PFA Font
    FileType::ext_pfb_otf,      4, {0x80, 0x01, 0x00, 0x00},                               // PFB OTF Font
    FileType::ext_ttfa,         4, {0x00, 0x01, 0x00, 0x00},                               // TTFA Font
    FileType::ext_otc,          4, {0x4F, 0x54, 0x43, 0x20},                               // OTC Font
    FileType::ext_ttc,          4, {0x74, 0x74, 0x63, 0x66},                               // TTC Font
    // Videos
    FileType::ext_avi,          4, {0x52, 0x49, 0x46, 0x46},                               // AVI Video
    FileType::ext_mpeg,         4, {0x00, 0x00, 0x01, 0xBA},                               // MPEG Video
    FileType::ext_webm,         4, {0x1A, 0x45, 0xDF, 0xA3},                               // WEBM Video
    FileType::ext_f4v,          4, {0x46, 0x4C, 0x56, 0x01},                               // F4V Video
    FileType::ext_mkv,          4, {0x1A, 0x45, 0xDF, 0xA3},                               // MKV Video
    FileType::ext_mov,          8, {0x00, 0x00, 0x00, 0x14, 0x66, 0x74, 0x79, 0x70},       // MOV Video
    FileType::ext_mp4,          8, {0x00, 0x00, 0x00, 0x18, 0x66, 0x74, 0x79, 0x70},       // MP4 Video
    FileType::ext_flv,          3, {0x46, 0x4C, 0x56},                                     // FLV Video
    FileType::ext_mpg,          4, {0x00, 0x00, 0x01, 0xBA},                               // MPG Video
    FileType::ext_m2ts,         4, {0x47, 0x41, 0x43, 0x30},                               // M2TS Video
    FileType::ext_ts,           4, {0x47, 0x49, 0x53, 0x54},                               // TS Video
    FileType::ext_m2v,          4, {0x00, 0x00, 0x01, 0xBA},                               // M2V Video
    FileType::ext_rmvb,         4, {0x2E, 0x52, 0x4D, 0x56},                               // RMVB Video
    FileType::ext_svi,          3, {0x53, 0x56, 0x49},                                     // SVI Video
    FileType::ext_mpg4,         8, {0x00, 0x00, 0x00, 0x20, 0x66, 0x74, 0x79, 0x70},       // MPG4 Video
    FileType::ext_3gp,          8, {0x00, 0x00, 0x00, 0x14, 0x66, 0x74, 0x79, 0x70},       // 3GP Video
    FileType::ext_asf,          16, {0x30, 0x26, 0xB2, 0x75, 0x8E, 0x66, 0xCF, 0x11,
                                       0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C},    // ASF Video
    FileType::ext_wmv,          16, {0x30, 0x26, 0xB2, 0x75, 0x8E, 0x66, 0xCF, 0x11,
                                       0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C},    // WMV Video
    FileType::ext_dv,           4, {0x44, 0x56, 0x30, 0x31},                               // DV Video
    FileType::ext_vrw,          3, {0x56, 0x52, 0x57},                                     // VRW Video
    FileType::ext_fli,          4, {0x46, 0x4C, 0x49, 0x46},                               // FLI Video
    FileType::ext_flic,         4, {0x46, 0x4C, 0x49, 0x43},                               // FLIC Video
    FileType::ext_swf,          3, {0x46, 0x57, 0x53},                                     // SWF Video
    FileType::ext_mng,          3, {0x4D, 0x4E, 0x47},                                     // MNG Video
    FileType::ext_hevc,         8, {0x00, 0x00, 0x00, 0x14, 0x66, 0x74, 0x79, 0x70},       // HEVC Video
    FileType::ext_h264,         8, {0x00, 0x00, 0x00, 0x1E, 0x66, 0x74, 0x79, 0x70},       // H264 Video
    FileType::ext_vp9,          4, {0x49, 0x49, 0x49, 0x49},                               // VP9 Video
    FileType::ext_mpeg4,        8, {0x00, 0x00, 0x00, 0x20, 0x66, 0x74, 0x79, 0x70},       // MPEG4 Video
    FileType::ext_mp4a,         8, {0x00, 0x00, 0x00, 0x18, 0x66, 0x74, 0x79, 0x70},       // MP4 Audio

    // Add more unique FileType and magic number entries as needed to reach at least 300
    //  ...

    // Unknown
    FileType::ext_unknown,      0, {}                                                      // Unknown File Type (always last position)
    
};

    bool     initializeTypeTable();
    FileType getFileType(const std::vector<uint8_t>& file);

}
#endif
