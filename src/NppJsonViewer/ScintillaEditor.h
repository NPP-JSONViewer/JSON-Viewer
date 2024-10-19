#pragma once
#include "Define.h"
#include <string>
#include <tuple>
#include <variant>

enum class ScintillaCode : short
{
    Unknown,
    Success,
    NotInitialized,
    NoSelection,
    MultiLineSelection
};

using ScintillaData = std::variant<std::string, ScintillaCode>;

class ScintillaEditor
{
public:
    explicit ScintillaEditor(const NppData& nppData);
    ~ScintillaEditor() = default;

    void RefreshViewHandle();
    auto GetJsonText() -> ScintillaData;
    void SetLangAsJson() const;
    bool IsJsonFile() const;
    auto GetCurrentFileName() const -> std::wstring;

    void ReplaceSelection(const std::string& text) const;

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

    void GoToLine(size_t nStartLine, size_t nLineToGo) const;
    void GoToPosition(size_t nStartLine, size_t nLineToGo, size_t nColumnIndex) const;

private:
    NppData m_NppData    = {};
    HWND    m_hScintilla = nullptr;

    size_t m_nStartPos = 0;
    size_t m_nEndPos   = 0;
};
