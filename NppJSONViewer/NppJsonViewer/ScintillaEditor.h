#pragma once
#include "Define.h"
#include <string>
#include <tuple>

class ScintillaEditor
{
public:
    explicit ScintillaEditor(const NppData &nppData);
    ~ScintillaEditor() = default;

    void        RefreshViewHandle();
    std::string GetJsonText();
    void        SetLangAsJson() const;
    bool        IsJsonFile() const;

    void ReplaceSelection(const std::string &text) const;

    void MakeSelection(size_t start, size_t end) const;
    auto GetSelectionStart() const -> size_t
    {
        return m_nStartPos;
    }
    auto GetSelectionEnd() const -> size_t
    {
        return m_nEndPos;
    }

    auto GetEOL() const -> unsigned;
    auto GetIndent() const -> std::tuple<char, unsigned>;

    void RefreshSelectionPos();

private:
    NppData m_NppData    = {};
    HWND    m_hScintilla = nullptr;

    size_t m_nStartPos = 0;
    size_t m_nEndPos   = 0;
};
