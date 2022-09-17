#pragma 

#include <string>
#include <memory>
#include "Define.h"
#include "JsonViewDlg.h"
#include "Notepad_plus_msgs.h"
#include "ShortcutCommand.h"
#include "AboutDlg.h"

class NppJsonPlugin
{
public:
	NppJsonPlugin();
	~NppJsonPlugin() = default;

	void PluginInit(HMODULE hModule);
	void PluginCleanup();

	// Notepad++ APIs to be implemented
	void SetInfo(const NppData& notpadPlusData);

	const TCHAR* GetPluginName() const;

	FuncItem* GetFuncsArray(int* nbF);

	void ProcessNotification(const SCNotification* notifyCode);

	LRESULT MessageProc(UINT msg, WPARAM wParam, LPARAM lParam);

	BOOL IsUnicode();

private:
	class Callback
	{
		friend class NppJsonPlugin;
		static NppJsonPlugin* m_pNppJsonPlugin;
	public:
		Callback() = default;
		~Callback() = default;

		static void ShowJsonDlg() { m_pNppJsonPlugin->ShowJsonDlg(); }
		static void FormatJson() { m_pNppJsonPlugin->FormatJson(); }
		static void CompressJson() { m_pNppJsonPlugin->CompressJson(); }
		static void OpenOptionDlg() { m_pNppJsonPlugin->OpenOptionDlg(); }
		static void ShowAboutDlg() { m_pNppJsonPlugin->ShowAboutDlg(); }
	};

	void SetMenuIcon();
	void InitCommandMenu();
	void InitToolbarIcon();

	void ToggleMenuItemState(int nCmdId, bool bVisible);

	void ShowJsonDlg();
	void FormatJson();
	void CompressJson();
	void OpenOptionDlg();
	void ShowAboutDlg();

private:
	HMODULE								m_hModule = nullptr;
	toolbarIcons						m_hMenuIcon = {};
	ShortcutCommand						m_shortcutCommands;
	NppData								m_NppData = {};
	std::unique_ptr<JsonViewDlg>		m_pJsonViewDlg = nullptr;
	std::unique_ptr<AboutDlg>			m_pAboutDlg = nullptr;
};

