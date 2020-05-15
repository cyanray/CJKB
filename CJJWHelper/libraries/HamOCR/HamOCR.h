#pragma once
#include <iostream>
#include <string>
#include <exception>
#include <list>
//#include <boost/filesystem.hpp>
#include "HDatHelper.h"
#include "CyImg/Pixels.h"
//using namespace boost::filesystem;
using std::string;
using std::exception;
using std::list;
class HamOCR
{
public:
	HamOCR() {}
	//void CreateDataFile(const string & folder, const string & output_file_path);
	void LoadDataFile(const string & filename);
	//完全检索数据库，找到汉明距离最小的数据
	string OCR(Pixels &img);
	//快速OCR，当汉明距离小于等于可接受的值时就退出检索
	//minHamming:可接受的汉明距离
	string OCR_fast(Pixels &img,unsigned int minHamming = 15);
	~HamOCR() {}
private:
	list<HDat> HDats;
};

