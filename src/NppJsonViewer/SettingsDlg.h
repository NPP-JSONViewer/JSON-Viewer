#pragma once
#include "DockingFeature/StaticDialog.h"
#include "Define.h"
#include <string>
#include <memory>

class SettingsDlg : public StaticDialog
{
public:
    SettingsDlg(HINSTANCE hInstance, HWND hParent, int nCmdId, const std::wstring& configPath, std::shared_ptr<Setting>& pSetting);
    ~SettingsDlg() = default;

    bool ShowDlg(bool bShow);

protected:
    virtual INT_PTR CALLBACK run_dlgProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

private:
    bool Apply();
    void destroy() override;
    bool ReadINI();
    bool WriteINI();
    void InitDlg();
    void ShowSpaceCountCtrls(bool bShow);

private:
    int                      m_nCmdId = -1;
    std::wstring             m_configPath;
    std::shared_ptr<Setting> m_pSetting = nullptr;
};
