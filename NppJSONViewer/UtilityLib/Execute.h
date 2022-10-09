#pragma once
#include <tchar.h>
#include <string>
#include <Windows.h>
#include <shellapi.h>

class Execute
{
public:
    Execute(const TCHAR *cmd, const TCHAR *args, const TCHAR *cDir, bool show = false);

    bool  Run(bool isElevationRequired = false);
    DWORD RunSync(bool isElevationRequired = false);

private:
    std::wstring     m_Command;
    std::wstring     m_Args;
    std::wstring     m_CurDir;
    bool             m_bShow      = false;
    SHELLEXECUTEINFO m_ShExecInfo = {};
};
