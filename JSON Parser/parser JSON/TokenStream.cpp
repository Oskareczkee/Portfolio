#pragma once
#include "TokenStream.h"
#include <codecvt>

using namespace std;

TokenStream::operator bool() const
{
	if (stream)
		return true;
	return false;
}

void TokenStream::putback(Token t)
{
	if (full) throw exception("Token Stream: calling putback while buffer is full");

	buffer = t;
	full = true;
}

Token TokenStream::get()
{
	if (full)
	{
		full = false;
		return buffer;
	}

	wchar_t ch;
	stream.get(ch);

	while (isspace(ch))
	{
		if(!stream)
			return Token((char)Tokens::eofToken);

		if (ch == '\n')
			line++;
		stream.get(ch);
	}

	switch (ch)
	{

	//single char tokens
	case (char)Tokens::arrayBegin:
	case (char)Tokens::arrayEnd:
	case (char)Tokens::mapBegin:
	case (char)Tokens::mapEnd:
	case (char)Tokens::value:
	case (char)Tokens::specialChar:
	case (char)Tokens::separator:
	case (char)Tokens::minus:
		return Token(ch);

	//load strings
	case (char)Tokens::string:
	{
		stream.get(ch);
		wstring str;
		while (ch != (char)Tokens::string)
		{
			if (ch == (char)Tokens::specialChar)
			{
				stream.get(ch);
				switch (ch)
				{
				case '\\':
					ch = '\\';
					break;
				case '"':
					ch = '"';
					break;
				case 't':
					ch = '\t';
					break;
				default:
					throw exception("Undefined special character !");
				}
			}

			str += ch;
			stream.get(ch);
		}

		return Token((char)Tokens::string, str);
	}

	//load numbers
	case '.':
	case'1': case'2':case'3':case'4':case'5':
	case'6':case'7':case'8':case'9':case'0':
	{
		stream.putback(ch);
		double val;
		stream >> val;
		return Token((char)Tokens::number, val);
	}
	default:
	{
		if (isalpha(ch))
		{
			wstring s;
			s += ch;

			while (stream.get(ch) && (isalpha(ch) || isdigit(ch) || ch == '_'))s += ch;

			stream.putback(ch);
			if (s == nullKey)return Token((char)Tokens::nullToken);
			else if (s == trueKey)return Token((char)Tokens::trueToken);
			else if (s == falseKey)return Token((char)Tokens::falseToken);

			else
			{
				string exc = "Undefined token '";
				exc += wstring_convert<codecvt_utf8<wchar_t>, wchar_t>().to_bytes(s);
				exc += '\'';
				throw exception(exc.c_str());
			}
		}
		else
		{
			string exc = "Undefined token '";
			exc += ch;
			exc += '\'';
			throw exception(exc.c_str());
		}
	}
	}
}

void TokenStream::ignore(wchar_t c)
{
	if (full && c == buffer.kind)
	{
		full = false;
		return;
	}

	full = false;
	wchar_t ch = 0;

	while (stream >> ch)
		if (ch == c)return;
}