#include "img.h"

using namespace std;

int main()
{
	img input1, input2;
	img output1_up, output1_down,
		output2_up, output2_down;

	input1.imread("input1.bmp");
	input2.imread("input2.bmp");

	input1.bilinear(output1_up, true);
	input1.bilinear(output1_down, false);
	input2.bilinear(output2_up, true);
	input2.bilinear(output2_down, false);

	output1_up.imwrite("output1_up.bmp");
	output1_down.imwrite("output1_down.bmp");
	output2_up.imwrite("output2_up.bmp");
	output2_down.imwrite("output2_down.bmp");

	return 0;

}