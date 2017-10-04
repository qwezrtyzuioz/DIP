#include <fstream>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <string>

using namespace std;

class img{

public:

	img();
	img(string in_name);
	~img();

	bool loaded;
	void imread(string in_name);
	void imwrie(string out_name);

private:
	char
		id[2],				//	identifier
		*data;				//	bmp data matrix

	int
		width,				//	width in pixel
		height;				//	height in pixel

	unsigned int
		file_size,			//	file size
		reserved,			//	reserved
		offset,				//	bitmap data offset
		header_size,		//	bitmap header size
		compress,			//	compression	
		data_size,			//	bitmap data size
		h_res,				//	horizontal resolution
		v_res,				//	vertical resolution
		use_col,			//	used color
		imp_col,			//	important colors
		real_size,			//	real file size
		*palette;			//	palette

	unsigned short int
		planes,				//	planes of bitmap
		pix_bit;			//	bit per pixel

};

img::img()
	:loaded(false){}

img::img(string in_name)
	: loaded(false)
{
	imread(in_name);
}

img::~img()
{
	if (loaded)
		delete[] data;
}

void img::imread(string in_name)
{
	
	ifstream fin;
	fin.open(in_name, ios::in | ios::binary);

	fin.seekg(0, fin.end);
	real_size = fin.tellg();
	cout << "The size of image is: " << real_size << " bytes" << endl;
	fin.seekg(0, fin.beg);

	fin.read(id, 2);
	fin.read((char*)&file_size, 4);
	fin.read((char*)&reserved, 4);
	fin.read((char*)&offset, 4);
	fin.read((char*)&header_size, 4);
	fin.read((char*)&width, 4);
	fin.read((char*)&height, 4);
	fin.read((char*)&planes, 2);
	fin.read((char*)&pix_bit, 2);
	fin.read((char*)&compress, 4);
	fin.read((char*)&data_size, 4);
	fin.read((char*)&h_res, 4);
	fin.read((char*)&v_res, 4);
	fin.read((char*)&use_col, 4);
	fin.read((char*)&imp_col, 4);

	data = new char[(real_size - offset)];
	fin.read(data, (real_size - offset));

	loaded = true;
	fin.close();

	cout << endl;
	cout << " Bitmap File Header:" << endl;
	cout << setw(20) << left << "Identifier" << id[0] << id[1] << endl;
	cout << setw(20) << left << "File Size" << file_size << endl;
	cout << setw(20) << left << "Reserved" << reserved << endl;
	cout << setw(20) << left << "Bitmap Data Offset" << offset << endl;
	cout << endl;
	cout << " Bitmap Info Header:" << endl;
	cout << setw(20) << left << "Bitmap Header Size" << header_size << endl;
	cout << setw(20) << left << "Width" << width << endl;
	cout << setw(20) << left << "Height" << height << endl;
	cout << setw(20) << left << "Planes" << planes << endl;
	cout << setw(20) << left << "Bits per Pixel" << pix_bit << endl;
	cout << setw(20) << left << "Compression" << compress << endl;
	cout << setw(20) << left << "Bitmap Data Size" << data_size << endl;
	cout << setw(20) << left << "H-Resolution" << h_res << endl;
	cout << setw(20) << left << "V-Resolution" << v_res << endl;
	cout << setw(20) << left << "Used Colors" << use_col << endl;
	cout << setw(20) << left << "Important Colors" << imp_col << endl;

}

void img::imwrie(string out_name)
{
	if (!loaded){
		cout << "ERROR!! this image is empty" << endl;
		return;
	}

	ofstream fout;
	fout.open(out_name, ios::out | ios::binary);

	fout.write(id, 2);
	fout.write((char*)&file_size, 4);
	fout.write((char*)&reserved, 4);
	fout.write((char*)&offset, 4);
	fout.write((char*)&header_size, 4);
	fout.write((char*)&width, 4);
	fout.write((char*)&height, 4);
	fout.write((char*)&planes, 2);
	fout.write((char*)&pix_bit, 2);
	fout.write((char*)&compress, 4);
	fout.write((char*)&data_size, 4);
	fout.write((char*)&h_res, 4);
	fout.write((char*)&v_res, 4);
	fout.write((char*)&use_col, 4);
	fout.write((char*)&imp_col, 4);
	fout.write((char*)data, data_size);
	fout.write((char*)&palette, offset - 54);
	fout.close();
}

int main()
{
	
	img input;

	input.imread("input1.bmp");
	input.imwrie("output1.bmp");


	











	system("PAUSE");
}