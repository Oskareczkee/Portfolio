#pragma once
#include "vector.h"
#include <iostream>

class myString : public myVector<char>
{
private:
public:
	//derive iterator from vector type
	using myVector::Iterator;
	using myVector::myVector;

	myString& operator+=(const char& ch)
	{
		this->push_back(ch);
		return *this;
	}

	//constructor that creates myString from char array (string)
	myString(const char* str): myVector()
	{
		while (*str)
			this->push_back(*str++);
	}

	bool operator==(const myString& other) const
	{
		//string are not the same size, and cannot be the same
		if (this->_size != other._size)
			return false;

		//check is every letter is the same
		for(int x =0;x<_size;x++)
		{
			if (arr[x] != other.arr[x])
				return false;
		}

		//all test have been passed
		return true;
	}

	bool operator!=(const myString& other) const {return !(*this == other);}

	bool operator>(const myString& other)const
	{
		int size = std::min(_size, other._size);

		for (int x = 0; x < size; x++)
		{
			if (arr[x] > other.arr[x])
				return true;
			else if (arr[x] == other.arr[x])
				continue;
			else return false;		
		}

		if (_size < other._size)
			return false;

		return true;
	}
	bool operator>=(const myString& other)const { return (*this > other) || (*this == other); }
	bool operator<(const myString& other)const { return !(*this > other); }
	bool operator<=(const myString& other)const { return (*this < other) || (*this == other); }

	//function that prints to the cout, stream can be change as well to file stream
	//this is due the lack of iterator implementation
	//if I had implemented iterator, i would override ostream operator<<
	friend std::ostream& operator<<(std::ostream& os, const myString& str)
	{
		for (int x = 0; x < str._size; x++)
			os << str.arr[x];

		return os;
	}
};
