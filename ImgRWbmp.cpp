#include "img.h"

using namespace std;

int main()
{
	img input1, input2;

	input1.imread("input1.bmp");
	input2.imread("input2.bmp");

	input1.imwrite("output1.bmp");
	input2.imwrite("output2.bmp");

	return 0;
}