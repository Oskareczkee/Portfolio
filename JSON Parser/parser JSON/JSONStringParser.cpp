#pragma once
#include "JSONStringParser.h"

void JSONStringParser::getMembers(Object* root)
{
	Token t = ts.get();

	if (t.kind != (char)Tokens::string)
		throw exception("Expected member name before value in map");

	wstring name = t.val;

	t = ts.get();
	if (t.kind != (char)Tokens::value)
		throw exception("Expected : before value in map");

	Object* o = getObject();

	root->_map->insert({ name, o });

	t = ts.get();
	if (t.kind == (char)Tokens::separator)
		getMembers(root);
	else
		ts.putback(t);
}

void JSONStringParser::getValues(Object* root)
{
	Object* o = getObject();

	root->_array->push_back(o);

	Token t = ts.get();
	if (t.kind == (char)Tokens::separator)
		getValues(root);
	else
		ts.putback(t);
}

Object* JSONStringParser::getObject()
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


void JSONStringParser::getContainers(Object* root)
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