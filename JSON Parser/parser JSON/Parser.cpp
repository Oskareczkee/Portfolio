#pragma once
#include "Parser.h"
#include "JSONChecker.h"
#include <memory>


void JSONParser::parseCommand(const wstring& command)
{
	wchar_t commandChar = command[0];

	wstring args;
	if (command.size() > 2)
		args = command.substr(2);

	//command letter size now does not matter
	commandChar = tolower(commandChar);

	switch (commandChar)
	{
	case 'h':
		showHelp();
		break;
	case 'o':
	{
		if (!openFile(args))
			wcout << "File could not be opened!\n";
		else
		{
			filePath = args;
			wcout << "File was sucessfully opened!\n";
		}
		break;
	}
	case 'c':
	{
		if (!checker.checkJSON(filePath))
			cerr<<checker.getErrorMessage() << "\n";
		else
			wcout << "File has been checked and its fine!\n";
		break;
	}
	case 'l':
		if (!file.is_open())
		{
			wcout << "There is no file opened and ready to read\n";
			break;
		}

		root = getObject();
		wcout << "Object has been successfully loaded into the memory!\n";
		break;
	case 'p':
	{
		if (root == nullptr)
		{
			wcout << "There is no object loaded in memory, use l command to load object into the memory!\n";
			break;
		}

		try
		{
			unique_ptr<Object> o = unique_ptr<Object>(JPath.getObject(args, root));
			wcout << *o << "\n";
		}
		catch (const exception& e)
		{
			wcerr << e.what() << "\n";
		}
		break;
	}
	case 'z':
	{
		if (root == nullptr)
		{
			wcout << "There is no object loaded in memory, use l command to load object into the memory!\n";
			break;
		}

		try
		{
			JPath.overwriteObject(args, root);
			writeToFile();
		}
		catch (exception& e)
		{
			cerr << e.what();
		}
	}
	default:
		break;
	}
}

void JSONParser::showHelp()
{
	wcout << L"\nJSON Parser, Created by Oskar NiedŸwiedzki 193602 2023\n"
		<< "h - show help\n"
		<< "o [file path] - opens file\n"
		<< "c - checks if loaded file is proper JSON file\n"
		<< "p [JSONPath] - gets and shows data at given path\n"
		<< "z [JSONPath] = [data] - gets and changes data at given path\n"
		<< "cls - clears the screen\n"
		<< "q - quits the program\n";
}

void JSONParser::writeToFile()
{
	file.close();
	file.clear();

	fileWrite.close();
	fileWrite.clear();
	fileWrite.open(filePath);

	if (!fileWrite)
		throw exception("File to write could not be opened");

	fileWrite << *root;

	fileWrite.close();
	fileWrite.clear();

	file.open(filePath);

	if (!file)
		throw exception("File to to open could not be opened");
	file.unsetf(ios_base::skipws);
}

bool JSONParser::openFile(const wstring& filePath)
{
	file.close();
	file.clear();
	file.open(filePath);
	file.unsetf(ios_base::skipws);

	ts.zeroLine();

	if (!file)
		return false;

	return true;
}

void JSONParser::getMembers(Object* root)
{
	Token t = ts.get();

	if (t.kind != (char)Tokens::string)
		throw exception("Expected member name before value in map");

	wstring name = t.val;

	t = ts.get();
	if (t.kind != (char)Tokens::value)
		throw exception("Expected : before value in map");

	Object* o = getObject();

	root->_map->insert({name, o});

	t = ts.get();
	if (t.kind == (char)Tokens::separator)
		getMembers(root);
	else
		ts.putback(t);
}

void JSONParser::getValues(Object* root)
{
	Object* o = getObject();

	root->_array->push_back(o);

	Token t = ts.get();
	if (t.kind == (char)Tokens::separator)
		getValues(root);
	else
		ts.putback(t);
}

Object* JSONParser::getObject()
{
	Token t = ts.get();

	Object* object = new Object();

	switch (t.kind)
	{
		//case (char)Tokens::specialChar:
		//break;
		case (char)Tokens::arrayBegin:
		case (char)Tokens::mapBegin:
			ts.putback(t);
			getContainers(object);
			break;
		case (char)Tokens::string:
			object->type = ValueType::String;
			object->_string = new wstring(t.val);
			break;
		case (char)Tokens::number:
			object->type = ValueType::Number;
			object->_double = new double(t.numberVal);
			break;
		case (char)Tokens::minus:
		{
			Token num = ts.get();
			if (num.kind != (char)Tokens::number)
				throw exception("Expected number after -");

			object->type = ValueType::Number;
			object->_double = new double(-num.numberVal);
			break;
		}
		case (char)Tokens::trueToken:
			object->type = ValueType::Literal;
			object->_bool = new bool(true);
			break;
		case (char)Tokens::falseToken:
			object->type = ValueType::Literal;
			object->_bool = new bool(false);
			break;
		case (char)Tokens::nullToken:
			object->type = ValueType::Literal;
			object->null = true;
			break;
		case (char)Tokens::eofToken:
			return object;
		default:
			throw exception("Expected value!");
	}

	return object;
}


void JSONParser::getContainers(Object* root)
{
	Token t = ts.get();
	
	switch (t.kind)
	{
	case (char)Tokens::arrayBegin:
		root->type = ValueType::Array;
		root->_array = new vector<Object*>();
		getValues(root);
		if (ts.get().kind != (char)Tokens::arrayEnd)
			throw exception("] Expected at the end of the array!");
		break;
	case (char)Tokens::mapBegin:
		root->type = ValueType::Map;
		root->_map = new map<wstring, Object*>();
		getMembers(root);
		if (ts.get().kind != (char)Tokens::mapEnd)
			throw exception("} Expected at the end of the map!");
		break;
	case (char)Tokens::eofToken:
		break;
	default:
		throw exception("Expected array or map");
	}
}

void JSONParser::goFileBeg()
{
	file.clear();
	file.seekg(0, ios::beg);
	ts.zeroLine();
}
