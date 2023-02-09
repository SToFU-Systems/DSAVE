#pragma once


class FileInformation
{
public:
    //  setters
    void setPath   (std::wstring_view filePath);
    void setPE     (const ntpe::IMAGE_NTPE_CONTEXT& nt);
    void setImport (const ntpe::IMPORT_LIST& imp);
    
private:
    std::wstring          m_FullPath; 
    ntpe::IMAGE_NTPE_DATA m_NtPE;
    ntpe::IMPORT_LIST     m_ImportList;
};

typedef std::map<std::wstring, FileInformation> FILE_INFORMATION_DATABASE;

extern FILE_INFORMATION_DATABASE g_FileInformationDB;