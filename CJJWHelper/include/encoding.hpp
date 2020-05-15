#pragma once
#include <string>
#include <iconv.h>
#include <exception>
#include <stdexcept>
using std::runtime_error;
using std::string;

static string GB2312_to_UTF8(const string& uStr)
{
	iconv_t conv = iconv_open("UTF-8//TRANSLIT", "GBK");

	if (conv == (iconv_t)-1) {
		throw runtime_error("An error occurred in iconv_open.");
	}

	string ret;
	ret.resize(uStr.size() * 2, '\0');		// 不知道转码之后的字符串长度是多少，直接扩容两倍防止溢出

	size_t inbytes = uStr.size();
	size_t outbytes = ret.size();
	char* inPtr = const_cast<char*>(uStr.c_str());
	char* outPtr = const_cast<char*>(ret.c_str());

	if (iconv(conv, &inPtr, &inbytes, &outPtr, &outbytes) == (size_t)-1)
	{
		iconv_close(conv);
		throw runtime_error("An error occurred in iconv.");
	}

	iconv_close(conv);

	return ret;
}