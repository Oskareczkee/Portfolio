#pragma once
#include "Utility.h"

void wait()
{
	wcin.ignore();
	wcin.get();
}

vector<wstring> split(wstring str, const wstring& delim)
{
	vector<wstring> output;
	size_t pos = 0;
	wstring token;
	while ((pos = str.find(delim)) != std::string::npos) {
		token = str.substr(0, pos);
		output.push_back(token);
		str.erase(0, pos + delim.length());
	}

	output.push_back(str);

	return output;
}