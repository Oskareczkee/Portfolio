#pragma once
#include <string>
#include <map>
#include <vector>
using namespace std;

enum class ValueType
{
	Array,
	Map,
	String,
	Number,
	Literal
};

class Object
{
public:
	ValueType type;
	wstring name;

	bool* _bool = nullptr;
	bool null = false;
	double* _double = nullptr;
	wstring* _string = nullptr;
	vector<Object*>* _array = nullptr;
	map<wstring, Object*>* _map = nullptr;

	void destroyContent()
	{
		delete _bool;
		delete _double;
		delete _array;
		delete _map;
		delete _string;
	}

	~Object()
	{
		delete _bool;
		delete _double;
		delete _array;
		delete _map;
		delete _string;
	}

	friend wostream& operator<<(wostream& os, const Object& o)
	{
		wostringstream wiss{};
		switch (o.type)
		{
		case ValueType::Array:
		{
			wiss.str(L"");
			os << "[\n";
			for (auto& a : *o._array)
				wiss << *a << ", ";
			wstring str = wiss.str();
			//remove 2 last chars
			str.pop_back();
			str.pop_back();
			os << str<<"\n]\n";
			return os;
		}
		case ValueType::Map:
		{
			wiss.str(L"");
			os << "{\n";
			for (auto& a : *o._map)
				wiss << "\"" << a.first << "\" : " << *a.second << ", ";
			wstring str = wiss.str();
			//remove 2 last chars
			str.pop_back();
			str.pop_back();
			os << str<<"\n}\n";
			return os;
		}
		case ValueType::String:
			os << '"';
			os << *o._string;
			os << '"';
			return os;
		case ValueType::Number:
			os << *o._double;
			return os;
		case ValueType::Literal:
		{
			if (o.null)
			{
				os << "null";
				return os;
			}
			else if (o._bool != nullptr)
			{
				os << (*o._bool ? "true" : "false");
				return os;
			}

			//otherwise just return os
			return os;
		}
		default:
			return os;
		}
	}
};
