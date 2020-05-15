/*
*该文件定义HDat包含的数据
*/
#pragma once
#include <string>
using std::string;
struct HDat
{
	HDat() :use_count(new unsigned int(1)), data(nullptr), length(0) {}
	HDat(const HDat& h)
	{
		++* h.use_count;
		this->data = h.data;
		this->length = h.length;
		this->str = h.str;
		this->use_count = h.use_count;
	}
	HDat& operator=(const HDat& h)
	{
		++* h.use_count;
		if (-- * use_count == 0)
		{
			delete[] data;
			delete use_count;
		}
		this->data = h.data;
		this->length = h.length;
		this->str = h.str;
		this->use_count = h.use_count;
		return *this;
	}
	string str = "";
	unsigned int length;
	unsigned char* data;
	~HDat()
	{
		if (-- * use_count == 0)
		{
			delete[] data;
			delete use_count;
		}
	}
private:
	unsigned int* use_count;
};