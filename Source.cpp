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
	void bilinear(img& store, string mode);
	void quant(int level);

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

void img::bilinear(img& store, string mode)
{
	int	step, num_pix, count;
	
	

	//these parameters in resized image the same as origin one.
	store.id[0] = id[0];
	store.id[1] = id[1];
	store.reserved = reserved;
	store.offset = offset;
	store.header_size = header_size;
	store.planes = planes;
	store.pix_bit = pix_bit;
	store.compress = compress;
	store.h_res = h_res;
	store.v_res = v_res;
	store.use_col = use_col;
	store.imp_col = imp_col;

	//these parameter in resized image change as size.
	//store.data_size = (num_pix* pix_bit) / 4;
	store.file_size = store.data_size + store.header_size;
	store.loaded = true;
	store.real_size = store.file_size;	
	store.data = new char[data_size];

}

void img::quant(int level)
{
	for (int i = 0; i < data_size; ++i){
		data[i] = data[i] >> level;
		data[i] = data[i] << level;
	}
}

int main()
{
	
	img input, resize;

	input.imread("input1.bmp");
	input.quant(5);
	input.imwrie("output1_3.bmp");
	











	system("PAUSE");
}