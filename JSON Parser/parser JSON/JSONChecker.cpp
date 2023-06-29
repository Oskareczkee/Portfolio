#pragma once
#include "JSONChecker.h"

void JSONChecker::getMembers()
{
	Token t = ts.get();

	if (t.kind != (char)Tokens::string)
		throw exception("Expected member name before value in map");

	wstring name = t.val;

	t = ts.get();
	if (t.kind != (char)Tokens::value)
		throw exception("Expected : before value in map");

	getObject();


	t = ts.get();
	if (t.kind == (char)Tokens::separator)
		getMembers();
	else
		ts.putback(t);
}

void JSONChecker::getValues()
{
	getObject();

	Token t = ts.get();
	if (t.kind == (char)Tokens::separator)
		getValues();
	else
		ts.putback(t);
}

void JSONChecker::getObject()
{
	Token t = ts.get();

	switch (t.kind)
	{
		//case (char)Tokens::specialChar:
		//break;
	case (char)Tokens::arrayBegin:
	case (char)Tokens::mapBegin:
		ts.putback(t);
		getContainers();
		break;
	case (char)Tokens::minus:
	{
		Token num = ts.get();
		if (num.kind != (char)Tokens::number)
			throw exception("Expected number after -");
		break;
	}
	case (char)Tokens::string:
	case (char)Tokens::number:
	case (char)Tokens::trueToken:
	case (char)Tokens::falseToken:
	case (char)Tokens::nullToken:
	case (char)Tokens::eofToken:
		return;
	default:
		throw exception("Expected value!");
	}
}


void JSONChecker::getContainers()
{
	Token t = ts.get();

	switch (t.kind)
	{
	case (char)Tokens::arrayBegin:
		getValues();
		if (ts.get().kind != (char)Tokens::arrayEnd)
			throw exception("] Expected at the end of the array!");
		break;
	case (char)Tokens::mapBegin:
		getMembers();
		if (ts.get().kind != (char)Tokens::mapEnd)
			throw exception("} Expected at the end of the map!");
		break;
	case (char)Tokens::eofToken:
		break;
	default:
		throw exception("Expected array or map");
	}
}

bool JSONChecker::checkJSON(const wstring& filePath)
{
	file.close();
	file.clear();
	file.open(filePath);
	file.unsetf(ios_base::skipws);
	
	if (!file)
	{
		error = "File could not be opened!";
		return false;
	}

	try
	{
		getObject();
	}
	catch (const std::exception& e)
	{
		ostringstream oss;
		oss << "An error occured at line: " << ts.getLine() << "\n" << e.what();
		error = oss.str();
		return false;
	}

	return true;
}

