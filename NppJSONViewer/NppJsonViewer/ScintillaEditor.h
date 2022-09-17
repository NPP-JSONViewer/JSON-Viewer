#pragma once
#include "Define.h"
#include <string>

class ScintillaEditor
{
public:
	ScintillaEditor(const NppData& nppData);
	~ScintillaEditor() = default;

	std::string GetJsonText();
	void SetLangAsJson() const;

	void ReplaceSelection(const std::string& text) const;

	void MakeSelection(int start, int end) const;
	auto GetSelectionStart() const->size_t { return m_nStartPos; }
	auto GetSelectionEnd() const->size_t { return m_nEndPos; }

private:
	void RefreshSelectionPos();

private:
	NppData m_NppData = {};
	HWND	m_hScintilla = nullptr;

	size_t	m_nStartPos = 0;
	size_t	m_nEndPos = 0;
};

