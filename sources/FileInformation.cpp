#include "stdafx.h"
#include "FileInformation.h"

FILE_INFORMATION_DATABASE g_FileInformationDB;

void FileInformation::setPath(std::wstring_view filePath)
{
    m_FullPath = tools::pathToFullLwr(filePath);
};

void FileInformation::setImport(const ntpe::IMPORT_LIST& imp)
{
    m_ImportList = imp;
}

void FileInformation::setPE(const ntpe::IMAGE_NTPE_CONTEXT& nt)
{
    auto data = ntpe::getNTPEData(nt);
    if (!data)
        return;
    m_NtPE = *data;
}