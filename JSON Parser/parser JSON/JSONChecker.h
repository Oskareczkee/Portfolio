#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "TokenStream.h"
#include "Utility.h"
#include "Object.h"

using namespace std;

/// <summary>
/// JSONChecker class is basically JSON parser, but it does not create object in memory
/// </summary>
class JSONChecker
{
private:
	wifstream file;
	TokenStream ts = TokenStream(file);
	string error ="";
private:
	void getContainers();
	void getValues();
	void getObject();
	void getMembers();
public:
	/// <summary>
	/// Please use try-catch construction, to get errors and display them
	/// To display errors just call exception.what();
	/// </summary>
	bool checkJSON(const wstring& filePath);
	string getErrorMessage() { return error; };
};
#pragma once
