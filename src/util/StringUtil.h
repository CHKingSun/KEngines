//
// Created by KingSun on 2018/07/14
//

#ifndef KENGINES_STRING_UTIL_H
#define KENGINES_STRING_UTIL_H

#include <string>

namespace KEngines { namespace KUtil {
	static std::wstring stringToWString(const std::string& str) {
		auto p_wstr = new wchar_t[str.size() + 1];
		std::mbstowcs(p_wstr, str.data(), str.size() + 1);
		std::wstring wstr(p_wstr);
		delete[] p_wstr;
		return wstr;
	}

	static std::string wstringToString(const std::wstring& wstr) {
		auto p_str = new char[wstr.size() * 2 + 1];
		std::wcstombs(p_str, wstr.data(), wstr.size() * 2 + 1);
		std::string str(p_str);
		delete[] p_str;
		return str;
	}

	static std::string getFileName(const std::string& path) {
		std::string::size_type s_index = path.find_last_of('\\');
		if (s_index == std::string::npos) s_index = path.find_last_of('/');
		if (s_index == std::string::npos) s_index = -1;
		std::string::size_type e_index = path.find_last_of('.');
		if (e_index == std::string::npos) e_index == path.length();
		return path.substr(s_index + 1, e_index - s_index - 1);
	}

	static std::string getFileSuffix(const std::string& path) {
		std::string::size_type index = path.find_last_of('.');
		if (index == std::string::npos) return path;
		return path.substr(index + 1);
	}

	static std::string getFileParent(const std::string& path) {
		std::string::size_type index = path.find_last_of('\\');
		if (index == std::string::npos) index = path.find_last_of('/');
		if (index == std::string::npos) return "/";
		return path.substr(0, index + 1);
	}
} }

#endif // !KENGINES_STRING_UTIL_H

