#include "Execute.h"

Execute::Execute(const TCHAR *cmd, const TCHAR *args, const TCHAR *cDir, bool show)
    : m_Command(cmd)
    , m_Args(args)
    , m_CurDir(cDir)
    , m_bShow(show)
{
    m_ShExecInfo.cbSize       = sizeof(SHELLEXECUTEINFO);
    m_ShExecInfo.fMask        = SEE_MASK_NOCLOSEPROCESS;
    m_ShExecInfo.hwnd         = NULL;
    m_ShExecInfo.lpFile       = m_Command.c_str();
    m_ShExecInfo.lpParameters = m_Args.c_str();
    m_ShExecInfo.lpDirectory  = m_CurDir.c_str();
    m_ShExecInfo.nShow        = show ? SW_SHOWNORMAL : SW_HIDE;
    m_ShExecInfo.hInstApp     = NULL;
}

bool Execute::Run(bool isElevationRequired)
{
    m_ShExecInfo.lpVerb = isElevationRequired ? TEXT("runas") : TEXT("open");

    auto shellExecRes = ::ShellExecuteEx(&m_ShExecInfo);
    return shellExecRes ? false : true;
}

DWORD Execute::RunSync(bool isElevationRequired)
{
    m_ShExecInfo.lpVerb = isElevationRequired ? TEXT("runas") : TEXT("open");

    ShellExecuteEx(&m_ShExecInfo);
    WaitForSingleObject(m_ShExecInfo.hProcess, INFINITE);

    DWORD exitCode = 0;
    if (::GetExitCodeProcess(m_ShExecInfo.hProcess, &exitCode) == FALSE)
    {
        exitCode = GetLastError();
    }

    return exitCode;
}
