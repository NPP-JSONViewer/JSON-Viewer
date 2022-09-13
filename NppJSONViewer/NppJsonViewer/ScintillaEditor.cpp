#include "ScintillaEditor.h"
#include <cassert>
//#include <algorithm>

ScintillaEditor::ScintillaEditor(const NppData& nppData) :
	m_NppData(nppData)
{
	int which = -1;
	::SendMessage(m_NppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, reinterpret_cast<LPARAM>(&which));
	assert(which != -1);
	if (which != -1)
		m_hScintilla = (which == 0) ? m_NppData._scintillaMainHandle : m_NppData._scintillaSecondHandle;
}

std::string ScintillaEditor::GetJsonText()
{
	if (!m_hScintilla)
		return std::string();

	// Adjust the selection position
	RefreshSelectionPos();

	// Get only selected text if any else select all text
	size_t asciiTextLen = m_nEndPos - m_nStartPos;
	char* chSelectedText = nullptr;
	if (asciiTextLen == 0)
	{
		asciiTextLen = ::SendMessage(m_hScintilla, SCI_GETLENGTH, 0, 0);
		::SendMessage(m_hScintilla, SCI_SETSELECTIONSTART, 0, 0);
		::SendMessage(m_hScintilla, SCI_SETSELECTIONEND, asciiTextLen, 0);
	}
	chSelectedText = new CHAR[asciiTextLen + 1];
	::SendMessage(m_hScintilla, SCI_GETSELTEXT, 0, reinterpret_cast<LPARAM>(chSelectedText));

	// Update selection position
	RefreshSelectionPos();

	return std::string(chSelectedText);
}

void ScintillaEditor::RefreshSelectionPos()
{
	m_nStartPos = ::SendMessage(m_hScintilla, SCI_GETSELECTIONSTART, 0, 0);
	m_nEndPos = ::SendMessage(m_hScintilla, SCI_GETSELECTIONEND, 0, 0);

	if (m_nEndPos < m_nStartPos)
		std::swap(m_nStartPos, m_nEndPos);
}
