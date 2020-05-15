#pragma once
#include "../Pixels.h"
namespace Cyan
{
	static void RGB_Binarize(Pixels & img,unsigned char value)
	{
		auto s = img.GetPixels();
		for (auto &p : s)
		{
			if (p.R() >= value && p.G() >= value && p.B() >= value)
			{
				p.SetRGB(255, 255, 255);
			}
			else
			{
				p.SetRGB(0, 0, 0);
			}
		}
		img.SetPixels(s);
	}
}