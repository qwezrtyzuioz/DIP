#include "img.h"

using namespace std;

int main()
{
	img input1, input2;
	
	input1.imread("input1.bmp");
	input2.imread("input2.bmp");

	input1.quant(1);
	input2.quant(1);
	input1.imwrite("output1_1.bmp");
	input2.imwrite("output2_1.bmp");

	input1.quant(3);
	input2.quant(3);
	input1.imwrite("output1_2.bmp");
	input2.imwrite("output2_2.bmp");

	input1.quant(5);
	input2.quant(5);
	input1.imwrite("output1_3.bmp");
	input2.imwrite("output2_3.bmp");
}