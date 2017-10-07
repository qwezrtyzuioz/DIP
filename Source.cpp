#include "img.h"
#include <cstdlib>

using namespace std;

int main()
{
	
	img input, resize;

	input.imread("output2_down.bmp");
	//input.quant(5);
	input.bilinear(resize, false);
	//input.imwrie("output2.bmp");
	resize.imwrie("output2_down.bmp");

	system("PAUSE");
}

