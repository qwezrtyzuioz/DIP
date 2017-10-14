#include "img.h"
#include <cstdlib>

using namespace std;

int main()
{

	img input, resize;

	input.imread("input2.bmp");
	//input.quant(5);
	input.bilinear(resize, false);
	//input.imwrie("output2.bmp");
	resize.imwrie("output2_down.bmp");

	system("PAUSE");
}
