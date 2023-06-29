#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "TokenStream.h"
#include "Utility.h"
#include "Object.h"

//JSON parser with ability to parse from string
class JSONStringParser {

private:
	wistringstream file{ };
	TokenStream ts = TokenStream(file);
	Object* root = nullptr;
	wstring objectString;
private:
	void getContainers(Object* root);
	void getValues(Object* root);
	void getMembers(Object* root);;
public:
	Object* getObject();
	JSONStringParser(const wstring& objectToParse) :objectString(objectToParse) { file.str(objectToParse); file.unsetf(ios::skipws); }
	~JSONStringParser() { delete root; };
};