#pragma once
class Pixel
{
public:
	Pixel() :R_(0), G_(0), B_(0) {}
	Pixel(unsigned char r, unsigned char g, unsigned char b) :R_(r), G_(g), B_(b) {}
	Pixel& SetRGB(unsigned char r, unsigned char g, unsigned char b) { R_ = r, G_ = g, B_ = b; return *this; }
	Pixel &R(unsigned char r) { R_ = r; return *this; }
	Pixel &G(unsigned char g) { G_ = g; return *this; }
	Pixel &B(unsigned char b) { B_ = b; return *this; }
	unsigned char R() const { return R_; }
	unsigned char G() const { return G_; }
	unsigned char B() const { return B_; }
	~Pixel() {}
private:
	unsigned char R_;
	unsigned char G_;
	unsigned char B_;
};