#pragma once
#include <regex>
#include <string>
const char WINDOWS_DISKS[] = { 'C', 'D', 'E', 'F' };
const wchar_t WWINDOWS_DISKS[] = { 'C', 'D', 'E', 'F' };
std::string RawPathToPolished(const std::string & rawPath);
std::wstring RawPathToPolished(const std::wstring & rawPath);
