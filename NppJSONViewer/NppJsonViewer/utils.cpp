#include "utils.h"

std::wstring getVersion(std::wstring filePath)
{
	std::wstring retVer;

	if (!filePath.empty() && ::PathFileExists(filePath.c_str()))
	{
		DWORD handle = 0;
		DWORD bufferSize = ::GetFileVersionInfoSize(filePath.c_str(), &handle);

		if (bufferSize > 0)
		{
			unsigned char* buffer = new unsigned char[bufferSize];
			::GetFileVersionInfo(filePath.c_str(), handle, bufferSize, buffer);

			VS_FIXEDFILEINFO* lpFileInfo = nullptr;
			UINT cbFileInfo = 0;
			VerQueryValue(buffer, TEXT("\\"), reinterpret_cast<LPVOID*>(&lpFileInfo), &cbFileInfo);
			if (cbFileInfo)
			{
				std::wostringstream os;
				os << ((lpFileInfo->dwFileVersionMS & 0xFFFF0000) >> 16);
				os << '.';
				os << (lpFileInfo->dwFileVersionMS & 0x0000FFFF);
				os << '.';
				os << ((lpFileInfo->dwFileVersionLS & 0xFFFF0000) >> 16);
				os << '.';
				os << (lpFileInfo->dwFileVersionLS & 0x0000FFFF);

				retVer = os.str();
			}
			delete[] buffer;
		}
	}

	return retVer;
}