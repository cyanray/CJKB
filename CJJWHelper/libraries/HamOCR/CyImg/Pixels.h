#pragma once
#include "pixel/pixel.h"
#include <vector>
#include <string>
using std::string;
using std::vector;
class Pixels
{
public:
	Pixels() :tRaw(nullptr), components(3), width(0), height(0) {}
	Pixels(const Pixels& p)
	{
		this->raw = p.raw;
		this->tRaw = nullptr;
		this->width = p.width;
		this->height = p.height;
		this->components = p.components;
	}
	Pixels& operator=(const Pixels& p)
	{
		this->raw = p.raw;
		this->tRaw = nullptr;
		this->width = p.width;
		this->height = p.height;
		this->components = p.components;
		return *this;
	}
	unsigned int width;
	unsigned int height;
	unsigned int components;
	void LoadFromJpegFile(const string& filename);
	void LoadFromJpegBytes(unsigned char bytes[], size_t length);
	void SaveAsJpeg(const string& filename);
	vector<Pixel>& GetPixels() { return raw; }
	void SetPixels(vector<Pixel> p) { raw = p; }
	//该指针由Pixels管理，请勿主动释放
	const unsigned char* GetRaw();
	~Pixels();
private:
	vector<Pixel> raw;
	unsigned char* tRaw;//调用GetRaw时，生成的bytes存入这里，每次调用都会删掉上次生成的tRaw
};

