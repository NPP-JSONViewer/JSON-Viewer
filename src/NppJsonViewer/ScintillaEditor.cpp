#include "ScintillaEditor.h"
#include <cassert>
#include <memory>

ScintillaEditor::ScintillaEditor(const NppData& nppData)
    : m_NppData(nppData)
{
    RefreshViewHandle();
}

void ScintillaEditor::RefreshViewHandle()
{
    int which = -1;
    ::SendMessage(m_NppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, reinterpret_cast<LPARAM>(&which));
    assert(which != -1);
    if (which != -1)
        m_hScintilla = (which == 0) ? m_NppData._scintillaMainHandle : m_NppData._scintillaSecondHandle;
}

auto ScintillaEditor::GetJsonText() -> ScintillaData
{
    if (!m_hScintilla)
        return ScintillaCode::NotInitialized;

    // Multi selection is not supported
    size_t nSelections = ::SendMessage(m_hScintilla, SCI_GETSELECTIONS, 0, 0);
    if (nSelections > 1)
        return ScintillaCode::MultiLineSelection;

    // Adjust the selection position
    RefreshSelectionPos();

    // Get only selected text if any else select all text
    size_t asciiTextLen = m_nEndPos - m_nStartPos;
    if (asciiTextLen == 0)
    {
        asciiTextLen = ::SendMessage(m_hScintilla, SCI_GETLENGTH, 0, 0);
        ::SendMessage(m_hScintilla, SCI_SETSELECTIONSTART, 0, 0);
        ::SendMessage(m_hScintilla, SCI_SETSELECTIONEND, asciiTextLen, 0);
    }
    std::unique_ptr<CHAR[]> chSelectedText = std::make_unique<CHAR[]>(asciiTextLen + 1);
    ::SendMessage(m_hScintilla, SCI_GETSELTEXT, 0, reinterpret_cast<LPARAM>(chSelectedText.get()));

    // Update selection position
    RefreshSelectionPos();

    return chSelectedText.get();
}

void ScintillaEditor::SetLangAsJson() const
{
    ::SendMessage(m_NppData._nppHandle, NPPM_SETCURRENTLANGTYPE, 0, LangType::L_JSON);
}

bool ScintillaEditor::IsJsonFile() const
{
    unsigned languageType = 0;
    ::SendMessage(m_NppData._nppHandle, NPPM_GETCURRENTLANGTYPE, 0, reinterpret_cast<LPARAM>(&languageType));
    return languageType == LangType::L_JSON;
}

auto ScintillaEditor::GetCurrentFileName() const -> std::wstring
{
    wchar_t fileName[MAX_PATH] {};
    ::SendMessage(m_NppData._nppHandle, NPPM_GETFILENAME, 0, reinterpret_cast<LPARAM>(&fileName));
    return fileName;
}

void ScintillaEditor::ReplaceSelection(const std::string& text) const
{
    ::SendMessage(m_hScintilla, SCI_REPLACESEL, 0, reinterpret_cast<LPARAM>(text.c_str()));
}

void ScintillaEditor::MakeSelection(size_t start, size_t end) const
{
    ::SendMessage(m_hScintilla, SCI_SETSEL, start, end);
}

auto ScintillaEditor::GetEOL() const -> unsigned
{
    LRESULT eolMode = ::SendMessage(m_hScintilla, SCI_GETEOLMODE, 0, 0);
    return static_cast<unsigned>(eolMode);
}

auto ScintillaEditor::GetIndent() const -> std::tuple<char, unsigned>
{
    bool     useTabs    = ::SendMessage(m_hScintilla, SCI_GETUSETABS, 0, 0);
    char     indentChar = useTabs ? '\t' : ' ';
    unsigned indentLen  = useTabs ? 1 : static_cast<unsigned>(::SendMessage(m_hScintilla, SCI_GETTABWIDTH, 0, 0));
    return std::tuple<char, unsigned>(indentChar, indentLen);
}

void ScintillaEditor::RefreshSelectionPos()
{
    m_nStartPos = ::SendMessage(m_hScintilla, SCI_GETSELECTIONSTART, 0, 0);
    m_nEndPos   = ::SendMessage(m_hScintilla, SCI_GETSELECTIONEND, 0, 0);

    if (m_nEndPos < m_nStartPos)
        std::swap(m_nStartPos, m_nEndPos);
}

void ScintillaEditor::GoToLine(size_t nStartLine, size_t nLineToGo) const
{
    ::SendMessage(m_hScintilla, SCI_GOTOLINE, nStartLine + nLineToGo, 0);
}

void ScintillaEditor::GoToPosition(size_t nStartLine, size_t nLineToGo, size_t nColumnIndex) const
{
    size_t lineStartPos = SendMessage(m_hScintilla, SCI_POSITIONFROMLINE, nStartLine + nLineToGo, 0);
    size_t targetPos    = lineStartPos + nColumnIndex;
    ::SendMessage(m_hScintilla, SCI_GOTOPOS, targetPos, 0);
}
