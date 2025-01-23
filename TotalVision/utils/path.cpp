#include "path.hpp"

std::string RawPathToPolished(const std::string& rawPath) {
	for (int diskname = 3; diskname < sizeof(WINDOWS_DISKS) / sizeof(char); diskname++) {
		std::string polishedPath = std::regex_replace(rawPath, std::regex(std::string("\\\\Device\\\\HarddiskVolume") + std::to_string(diskname)), WINDOWS_DISKS[diskname - 3] + std::string(":"));
		if (polishedPath != "") return polishedPath;
	}
	return "";
}
std::wstring RawPathToPolished(const std::wstring& rawPath) {
	for (int diskname = 3; diskname < sizeof(WWINDOWS_DISKS) / sizeof(wchar_t); diskname++) {
		std::wstring polishedPath = std::regex_replace(rawPath, std::wregex(std::wstring(L"\\\\Device\\\\HarddiskVolume") + std::to_wstring(diskname)), WWINDOWS_DISKS[diskname - 3] + std::wstring(L":"));
		if (polishedPath != L"") return polishedPath;
	}
	return L"";
}