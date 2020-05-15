#pragma once
#include <fstream>
#include "HDat.h"
using std::fstream;
class HDatHelper
{
public:
	HDatHelper() {}
	//打开一个HDat文件
	void Open(const string & filename);
	//创建一个HDat文件(如文件已存在则覆盖)
	void Create(const string & filename);
	bool AtEOF() { return (hdFile.peek() == EOF) || hdFile.eof(); }
	//读取一个HDat数据
	HDat ReadOne();
	//在末尾加入一个HDat数据
	void AppendOne(HDat &hdat);
	~HDatHelper() { hdFile.close(); }
private:
	fstream hdFile;
};

