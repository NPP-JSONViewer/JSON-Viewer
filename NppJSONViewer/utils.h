#pragma once
#include <sstream>
#include <string>
#include <Shlwapi.h>

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

std::wstring getVersion(std::wstring filePath);
