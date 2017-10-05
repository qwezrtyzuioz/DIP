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
	void bilinear(img& store, bool is_up);
	void quant(int level);

private:
	char
		id[2],				//	identifier
		*data,				//	bmp data matrix
		*palette;			//	palette

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
		real_size;			//	real file size

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
	real_size = (unsigned int)fin.tellg();
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

	data = new char[(real_size - offset) + 2 * width * pix_bit / 8];
	palette = new char[(offset - 54)];

	fin.read(palette, (offset - 54));
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

void img::bilinear(img& store, bool is_up)
{
	int	new_width, new_height;

	if (is_up){
		new_width = int(width* 1.5);
		new_height = int(height* 1.5);
	}
	else{
		new_width = int(width/ 1.5);
		new_height = int(height/ 1.5);
	}

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
	store.data_size = (new_width* new_height* pix_bit) / 4;
	store.file_size = store.data_size + store.header_size;
	store.width = new_width;
	store.height = new_height;
	store.loaded = true;
	store.real_size = store.file_size;	
	store.data = new char[store.data_size + 2 * width];

	int 
		step,
		qx, qy;

	unsigned int
		r1[3], r2[3], inp[3],
		pos, 
		pos1, pos2;

	double 
		a, b,
		cor_x, cor_y;

	step = pix_bit / 8;

	for (int y = 0; y < new_height; ++y){
		for (int x = 0; x < new_width; ++x){
			
			if (is_up){
				cor_x = (double)x / 1.5;
				cor_y = (double)y / 1.5;
			}
			else{
				cor_x = (double)x * 1.5;
				cor_y = (double)y * 1.5;
			}

			qx = (int)cor_x;
			qy = (int)cor_y;

			pos1 = qx + qy* width;
			pos2 = (qx + 1) + qy*width;
			a = abs(qx - cor_x);
			b = abs(1 - a);
			for (int i = 0; i < 3; ++i){
				r1[i] = a* data[pos2 * 3 + i] + b* data[pos1 * 3 + i];
			}
			pos1 = qx + (qy + 1)* width;
			pos2 = (qx + 1) + (qy + 1)*width;
			for (int i = 0; i < 3; ++i){
				r2[i] = a* data[pos2 * 3 + i] + b* data[pos1 * 3 + i];
			}
			a = abs(qy - cor_y);
			b = (1 - a);
			for (int i = 0; i < 3; ++i){
				inp[i] = a* r2[i] + b* r1[i];
			}
			for (int i = 0; i < 3; ++i){
				pos = (x + y* store.width) * 3;
				store.data[pos + i] = inp[i];

				
			}

		}
	}

}

void img::quant(int level)
{
	for (unsigned int i = 0; i < data_size; ++i){
		data[i] = data[i] >> level;
		data[i] = data[i] << level;
	}
}

int main()
{
	
	img input, resize;

	input.imread("input2.bmp");
	//input.quant(5);
	input.bilinear(resize, true);
	//input.imwrie("output2.bmp");
	resize.imwrie("ourput2_up.bmp");











	system("PAUSE");
}