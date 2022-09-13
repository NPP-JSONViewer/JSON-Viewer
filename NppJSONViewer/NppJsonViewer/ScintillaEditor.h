#pragma once
#include "Define.h"
#include <string>

class ScintillaEditor
{
public:
	ScintillaEditor(const NppData& nppData);
	~ScintillaEditor() = default;

	std::string GetJsonText();

private:
	void RefreshSelectionPos();

private:
	NppData m_NppData = {};
	HWND	m_hScintilla = nullptr;

	size_t	m_nStartPos = 0;
	size_t	m_nEndPos = 0;
};

