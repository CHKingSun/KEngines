//
// Created by KingSun on 2018/07/14
//

#ifndef KENGINES_STRING_UTIL_H
#define KENGINES_STRING_UTIL_H

#include <string>

namespace KEngines { namespace KUtil {
	static std::wstring stringToWString(const std::string& str) {
		auto p_wstr = new wchar_t[str.size()];
		std::mbstowcs(p_wstr, str.data(), str.size());
		std::wstring wstr(p_wstr);
		delete[] p_wstr;
		return wstr;
	}

	static std::string wstringToString(const std::wstring& wstr) {
		auto p_str = new char[wstr.size() * 2];
		std::wcstombs(p_str, wstr.data(), wstr.size() * 2);
		std::string str(p_str);
		delete[] p_str;
		return str;
	}
} }

#endif // !KENGINES_STRING_UTIL_H

