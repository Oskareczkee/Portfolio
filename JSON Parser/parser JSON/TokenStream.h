#pragma once
#include <iostream>

using namespace std;

enum class Tokens
{
	string = '"',
	arrayBegin = '[',
	arrayEnd = ']',
	mapBegin = '{',
	mapEnd = '}',
	value = ':',
	specialChar = '\\',
	number = 'N',
	minus='-',
	trueToken='T',
	falseToken='F',
	separator=',',
	nullToken='\0',
	eofToken=3
};

const wstring trueKey = L"true";
const wstring falseKey = L"false";
const wstring nullKey = L"null";

class Token {
public:
	wchar_t kind;
	wstring val;
	double numberVal=0;

	Token(wchar_t ch):kind(ch){}
	Token(wchar_t ch, wstring value):kind(ch), val(value){}
	Token(wchar_t ch, double value):kind(ch), numberVal(value){}
	Token(wchar_t ch, wstring value, string Name):kind(ch), val(value){}
	Token(wchar_t ch, double value, string Name) :kind(ch), numberVal(value) {}
};

class TokenStream
{
	public:
		TokenStream(wistream& inputStream):stream(inputStream), buffer(0), full(false) {}
		
		Token get();
		void putback(Token t);
		void ignore(wchar_t c);
		explicit operator bool() const;

		void zeroLine() { line = 0; }
		int getLine() { return line; }
	private:
		wistream& stream;
		Token buffer;
		bool full;
		int line = 0;
};


