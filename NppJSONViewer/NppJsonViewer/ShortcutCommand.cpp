#include "ShortcutCommand.h"
#include <tchar.h>

ShortcutCommand::ShortcutCommand(int nCommandCount)
    : m_nCmdCount(nCommandCount)
    , m_pFuncItem(std::make_unique<FuncItem[]>(nCommandCount))
    , m_pShortcutKeys(std::make_unique<ShortcutKey[]>(nCommandCount))
{
}

bool ShortcutCommand::SetCommand(CallBackID id, const TCHAR *cmdName, const PFUNCPLUGINCMD pFunc, bool checkOnInit)
{
    int nIndex = static_cast<int>(id);

    if (m_nCmdCount <= nIndex || !pFunc)
        return false;

    _tcscpy_s(m_pFuncItem[nIndex]._itemName, cmdName);
    m_pFuncItem[nIndex]._pFunc      = pFunc;
    m_pFuncItem[nIndex]._init2Check = checkOnInit;
    m_pFuncItem[nIndex]._pShKey     = &m_pShortcutKeys[nIndex];

    return true;
}

bool ShortcutCommand::SetShortCut(CallBackID id, const ShortcutKey &scKey)
{
    int nIndex = static_cast<int>(id);

    if (m_nCmdCount <= nIndex)
        return false;

    m_pShortcutKeys[At(id)] = scKey;

    return true;
}
