/*
BodySlide and Outfit Studio
See the included LICENSE file
*/

#include "PlatformUtil.h"
#include <wx/log.h>
#include <wx/utils.h>
#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/tokenzr.h>
#include <wx/filename.h>

#include <iostream>
#include <fstream>
#include <algorithm>
#include <filesystem>

namespace {
std::string backslash_to_slash(const std::string& s) {
	std::string sc(s);
	size_t len = sc.length();
	for (size_t i = 0; i < len; ++i)
		if (sc[i] == '\\')
			sc[i] = '/';
	return sc;
}
} // namespace

#include <algorithm>
#include <fstream>
#include <string>

#ifndef _WINDOWS
#include <filesystem>
namespace fs = std::filesystem;
#endif

namespace PlatformUtil {
#ifdef _WINDOWS
// ACP wide to multibyte
std::string WideToMultiByteACP(const std::wstring& wstr) {
	if (wstr.empty())
		return std::string();

	int size_needed = WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), nullptr, 0, nullptr, nullptr);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, nullptr, nullptr);
	return strTo;
}

// UTF-8 multibyte to wide
std::wstring MultiByteToWideUTF8(const std::string& str) {
	if (str.empty())
		return std::wstring();

	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), nullptr, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}
#endif

void OpenFileStream(std::fstream& file, const std::string& fileName, std::ios_base::openmode mode) {
#ifdef _WINDOWS
	// Convert to std::wstring on Windows only
	file.open(MultiByteToWideUTF8(fileName).c_str(), mode);
#else
	std::string fn_nobs = backslash_to_slash(fileName);
	file.open(fn_nobs.c_str(), mode);
#endif
}

bool FileExists(const std::string& fileName) {
	std::fstream file;
	PlatformUtil::OpenFileStream(file, fileName, std::ios::in | std::ios::binary);

	if (!file)
		return false;

	return true;
}

// Provide std::wstring function for Windows
#ifdef _WINDOWS
void OpenFileStream(std::fstream& file, const std::wstring& fileName, unsigned int mode) {
	file.open(fileName.c_str(), mode);
}

bool FileExists(const std::wstring& fileName) {
	std::fstream file;
	PlatformUtil::OpenFileStream(file, fileName, std::ios::in | std::ios::binary);

	if (!file)
		return false;

	return true;
}
#endif
#ifndef _WINDOWS
std::string FindFilePathCaseInsensitive(const std::string& path) {
	std::string p = backslash_to_slash(path);
	if (fs::exists(p)) return p;

	fs::path current = "/";
	fs::path target(p);
	
	// Se for caminho relativo, começa do diretório atual
	if (!target.is_absolute())
		current = fs::current_path();
	else
		current = target.root_path();

	for (auto& part : target) {
		if (part == target.root_path() || part.string() == "/") continue;
		
		bool found = false;
		if (fs::exists(current / part)) {
			current /= part;
			found = true;
		} else {
			// Busca insensível no diretório atual
			std::string partLower = part.string();
			std::transform(partLower.begin(), partLower.end(), partLower.begin(), ::tolower);
			
			if (fs::is_directory(current)) {
				for (const auto& entry : fs::directory_iterator(current)) {
					std::string entryName = entry.path().filename().string();
					std::string entryLower = entryName;
					std::transform(entryLower.begin(), entryLower.end(), entryLower.begin(), ::tolower);
					
					if (entryLower == partLower) {
						current /= entryName;
						found = true;
						break;
					}
				}
			}
		}
		if (!found) {
			wxLogMessage("FindFilePathCaseInsensitive: Failed to find part '%s' in '%s'", part.string(), current.string());
			return p;
		}
	}
	
	if (current.string() != p) {
		wxLogMessage("FindFilePathCaseInsensitive: Resolved '%s' to '%s'", p, current.string());
	}

	return current.string();
}
#else
std::string FindFilePathCaseInsensitive(const std::string& path) {
	return path;
}
#endif
} // namespace PlatformUtil
