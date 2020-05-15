#include "HamOCR.h"
#include "CyImg/algorithm/Hamming/Hamming.hpp"
//void HamOCR::CreateDataFile(const string & folder, const string & output_file_path)
//{
//	HDatHelper hh;
//	hh.Create(output_file_path);
//	using namespace std;
//	try
//	{
//		path p_folder(folder);
//		if (exists(p_folder))
//		{
//			if (is_directory(p_folder))
//			{
//				for (auto& d : directory_iterator(p_folder))
//				{
//					if (is_directory(d.path()))
//					{
//						string str = d.path().filename().string();//HDat.str字段
//						for (auto& x : directory_iterator(d.path()))
//						{
//							path t = x.path();
//							string suffix = t.extension().string();
//							HDat tHDat;
//							Pixels img;
//							if (suffix == ".jpg" || suffix == ".jpeg")
//							{
//								img.LoadFromJpegFile(t.string());
//							}
//							else
//							{
//								continue;
//							}
//							tHDat.str = str;
//							tHDat.length = img.height * img.width *img.components;
//							//不能直接把Pixels的指针直接给tHDat
//							//Pixels类会释放GetRaw所得的指针
//							//所以只能把数据复制一次
//							tHDat.data = new unsigned char[tHDat.length];
//							memcpy(tHDat.data, img.GetRaw(), tHDat.length);
//							hh.AppendOne(tHDat);
//						}
//					}
//				}
//			}
//			else
//			{
//				throw exception((folder + " is not a directory.").data());
//			}
//		}
//	}
//	catch (const std::exception&ex)
//	{
//		throw ex;
//	}
//}

void HamOCR::LoadDataFile(const string & filename)
{
	HDatHelper hh;
	hh.Open(filename);
	while (!hh.AtEOF())
	{
		HDats.push_back(hh.ReadOne());
	}
}

string HamOCR::OCR(Pixels & img)
{
	unsigned int minVal = -1;//初始化为最大值
	string cVal;
	for (auto& x : HDats)
	{
		unsigned int t = Cyan::CalcHamming(img, x.data);
		if (t<minVal)
		{
			minVal = t;
			cVal = x.str;
		}
	}
	return cVal;
}

string HamOCR::OCR_fast(Pixels & img, unsigned int minHamming)
{
	unsigned int minVal = -1;//初始化为最大值
	string cVal;
	for (auto& x : HDats)
	{
		unsigned int t = Cyan::CalcHamming(img, x.data);
		if (t < minVal)
		{
			minVal = t;
			cVal = x.str;
			if (minVal <= minHamming) break;
		}
	}
	return cVal;
}
