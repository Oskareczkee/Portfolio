#pragma once
#include "Object.h"
#include "Utility.h"
#include <sstream>
#include <codecvt>
#include <memory>
#include "JSONStringParser.h"

class JSONPath
{
private:
	struct JPATHValue
	{
		int arrayIndex = -1;
		wstring mapKey;
	};

	Object* rootRef;
public:
	~JSONPath()
	{
		//we dont want to delete rootRef, as it is only reference, parser should carry about Object memory
		rootRef = nullptr;
	}


	Object* getObject(const wstring& JSONPathString, Object* root)
	{
		rootRef = root;

		wistringstream iss{ JSONPathString };
		wchar_t beg;
		iss >> beg;
		if (beg != '$')
			throw exception("JPath: JPath string has to begin with '$' character");

		wchar_t ch;
		while (iss)
		{
			JPATHValue value;
			bool key = false;

			iss >> ch;

			if (!iss)
				break;

			if (ch != '[')
				throw exception("JPath: expected '[' at the beginning of expression");
			iss >> ch;
			//load map keys 
			if (ch == '"')
			{
				wstring str;
				while (iss >> ch && ch != '"')
					str += ch;
				value.mapKey = str;
				key = true;
			}
			//load array index
			else
			{
				iss.putback(ch);

				int index = -1;
				if (!(iss >> index))
					throw exception("JPATH: Array index is invalid!");
				value.arrayIndex = index;
				key = false;
			}

			iss >> ch;
			if (ch != ']')
				throw exception("JPath: Expected ] at the end of the expression");

			if (key)
			{
				if (rootRef->type == ValueType::Map)
				{
					auto mapPair = rootRef->_map->find(value.mapKey);
					if (mapPair != rootRef->_map->end())
						rootRef = mapPair->second;
					else
					{
						string ref = "JPath: Value '";
						ref += wstring_convert<codecvt_utf8<wchar_t>, wchar_t>().to_bytes(value.mapKey);
						ref += "' Was not found in the map";
						throw exception(ref.c_str());
					}
				}
				else
				{
					string ref = "JPath: Object is not map type and value '";
					ref += wstring_convert<codecvt_utf8<wchar_t>, wchar_t>().to_bytes(value.mapKey);
					ref += "' cannot be used as a key";
					throw exception(ref.c_str());
				}
			}
			else
			{
				if (rootRef->type == ValueType::Array)
				{
					if (rootRef->_array->size() < value.arrayIndex && value.arrayIndex >= 0)
						//somehow i couldnt use [] to get the pointer
						rootRef = rootRef->_array->at(value.arrayIndex);
					else
						throw exception("JPath: Array index out of bounds!");
				}
				else
					throw exception("JPath: Object is not an array type");
			}
		}

		return rootRef;
	}

	void overwriteObject(const wstring& JSONPathString, Object* root)
	{
		auto tokens = split(JSONPathString, L"=");

		auto pos = JSONPathString.find(L'=');

		wstring JPathStr = JSONPathString.substr(0, pos);
		wstring ObjectStr = JSONPathString.substr(pos+1, JSONPathString.size() - pos);


		if (tokens.size() != 2)
			throw exception("JPath: JPath string has more or less than one '='\n");

		JSONStringParser stringParser(ObjectStr);
		try
		{
			Object* objRef = getObject(JPathStr, root);
			Object* newObj = stringParser.getObject();

			objRef->destroyContent();
			*objRef = *newObj;
			cout << "\nObject was sucessfully overwritten! \n";
		}

		catch (exception& e)
		{
			cerr << e.what();
		}
	}



};