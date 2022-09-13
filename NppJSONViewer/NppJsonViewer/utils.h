#pragma once
#include <sstream>
#include <string>
#include <Shlwapi.h>

template<typename T>
constexpr auto SSTR(T x) { return (std::ostringstream() << std::dec << x).str(); }

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

std::wstring getVersion(std::wstring filePath);

