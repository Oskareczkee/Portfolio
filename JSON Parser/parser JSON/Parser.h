#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <sstream>
#include "TokenStream.h"
#include "Utility.h"
#include "Object.h"
#include "JSONPath.h"
#include "JSONChecker.h"
#include "JSONStringParser.h"

using namespace std;

/*
* Grammar:
* Top:
*	-Array
*	-Map
* 
* Array:
*	"["Object ","..."]"
* Map:
*	"["Member ","..."]"
* 
* Object:
*	value
*	map
*	array
* Member:
* "name": value
* 
* value:
*	-string
*	-number
* 
*/

class JSONParser
{
private:
	wifstream file{""};
	wofstream fileWrite{""};
	TokenStream ts = TokenStream(file);
	Object* root = nullptr;
	wstring filePath;
	JSONChecker checker;
	JSONPath JPath;
private:
	void showHelp();
	void getContainers(Object* root);
	void getValues(Object* root);
	void getMembers(Object* root);
	Object* getObject();
	void goFileBeg();
	void writeToFile();
public:
	void parseCommand(const wstring& command);
	bool openFile(const wstring& filePath);
	~JSONParser() { delete root;};
};
