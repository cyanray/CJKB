#pragma once
#include "../../Pixels.h"
namespace Cyan
{
	static unsigned int CalcHamming(Pixels & imgA, Pixels & imgB)
	{
		unsigned int len = imgA.width * imgA.height * imgA.components, t = 0;
		const unsigned char *a = imgA.GetRaw(), *b = imgB.GetRaw();
		for (size_t i = 0; i < len; i+=3)
		{
			if (a[i] != b[i] || a[i+1] != b[i+1] || a[i+2] != b[i+2])
			{
				++t;
			}
		}
		return t;
	}
	static unsigned int CalcHamming(Pixels & imgA, unsigned char *imgB)
	{
		unsigned int len = imgA.width * imgA.height * imgA.components, t = 0;
		const unsigned char *a = imgA.GetRaw(), *b = imgB;
		for (size_t i = 0; i < len; i += 3)
		{
			if (a[i] != b[i] || a[i + 1] != b[i + 1] || a[i + 2] != b[i + 2])
			{
				++t;
			}
		}
		return t;
	}
}