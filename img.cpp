#include "img.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>


img::img()
	:loaded(false){}	//	Note the image is empty
img::img(string in_name)
	: loaded(true)
{
	// ------------------------------------------------------------------------------
	//   File reading
	// ------------------------------------------------------------------------------
	imread(in_name);
}
img::~img()
{
	// ------------------------------------------------------------------------------
	//   Data Deloaction
	// ------------------------------------------------------------------------------

	if (loaded)
		delete[] data;
}

void img::imread(string in_name)
{

	// ------------------------------------------------------------------------------
	//   File opening
	// ------------------------------------------------------------------------------

	ifstream fin;
	fin.open(in_name, ios::in | ios::binary);

	if (!fin.is_open()){
		cout << "ERROR!! Image Doesn't Exist Can't be Loaded!! " << endl;
		return;
	}

	// ------------------------------------------------------------------------------
	//   File reading
	// ------------------------------------------------------------------------------
	/*
	* Using seekg function analysis the size of file first. Then read these
	* parameters in turn ( mind the order ).
	*/

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

	//	The size of data and palette depends. Using dynamic allocation. 
	data = new char[(real_size - offset) + 2 * width * pix_bit / 8];
	palette = new char[(offset - 54)];

	fin.read(palette, (offset - 54));
	fin.read(data, (real_size - offset));

	//	Note the image is no longer a empty image. Then close the file
	loaded = true;
	fin.close();

	// ------------------------------------------------------------------------------
	//	Information Printing
	// ------------------------------------------------------------------------------

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
	// ------------------------------------------------------------------------------
	//   Checking
	// ------------------------------------------------------------------------------

	if (!loaded){
		cout << "ERROR!! Empty Image Can't be Writen!!" << endl;
		return;
	}

	// ------------------------------------------------------------------------------
	//   File writing
	// ------------------------------------------------------------------------------
	/*
	* Open a file in binary methond. Then wirte these parameters in turn.
	*/

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
	fout.write((char*)&palette, offset - 54);
	fout.write((char*)data, data_size);

	fout.close();
}

void img::bilinear(img& store, bool is_up)
{
	int	new_width, new_height;

	if (!loaded){
		cout << "ERROR!! Empty Image Can't be Resize!!" << endl;
		return;
	}

	if (is_up){
		new_width = int(width* 1.5);
		new_height = int(height* 1.5);
		new_width = (new_width / 4) * 4;
	}
	else{
		new_width = int(width / 1.5);
		new_height = int(height / 1.5);
		new_width = (new_width / 4) * 4;
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
	store.palette = new char[(offset - 54)];
	for (unsigned int i = 0; i < offset - 54; ++i)
		store.palette[i] = palette[i];
	//these parameter in resized image change as size.
	store.width = new_width;
	store.height = new_height;
	store.data_size = (store.width* store.height* store.pix_bit) / 8;
	store.file_size = store.data_size + store.offset;
	store.loaded = true;
	store.real_size = store.file_size;
	store.data = new char[store.data_size + 2 * store.pix_bit* store.width / 8];


	int
		step,
		r1[4], r2[4], inp[4],
		qx, qy;

	unsigned int
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

			//	caculate the real position of reference points to r1
			pos1 = (qx + qy* width)* step;
			pos2 = ((qx + 1) + qy* width)* step;
			//	caculate the ratio of distance from r1 to both reference points
			a = abs(cor_x - qx);
			b = abs(1 - a);

			//	intopolate the r1 according to the reference points and the ratio
			for (int i = 0; i < step; ++i){
				r1[i] = int(a* (unsigned char)data[pos2 + i] + b* (unsigned char)data[pos1 + i]);
			}

			//	caculate the real position of reference points to r2
			pos1 = (qx + (qy + 1)* width)* step;
			pos2 = ((qx + 1) + (qy + 1)*width)* step;
			//	the ratio to both reference points the same as r1
			//	intopolate the r1 according to the reference points and the ratio
			for (int i = 0; i < step; ++i){
				r2[i] = int(a* (unsigned char)data[pos2 + i] + b* (unsigned char)data[pos1 + i]);
			}

			a = abs(cor_y - qy);
			b = abs(1 - a);
			for (int i = 0; i < step; ++i){
				inp[i] = a* (double)r2[i] + b* (double)r1[i];
			}
			for (int i = 0; i < step; ++i){
				pos = (x + y* store.width) * step;
				store.data[pos + i] = inp[i];
				if (inp[i]<0) cout << inp[i] << endl;
			}

		}
	}

}

void img::quant(int level)
{

	// ------------------------------------------------------------------------------
	//   Checking
	// ------------------------------------------------------------------------------

	if (!loaded){
		cout << "ERROR!! Empty Image Can't be Quantize" << endl;
		return;
	}

	// ------------------------------------------------------------------------------
	//   Quantize Function
	// ------------------------------------------------------------------------------
	/*
	* Shift the data store in bmp to right then to lefe. This operation will
	* discard unsignificant bit and turn them to zero.
	* The level is the compress ratio. 1 is no Compress, 7 is Thresholding.
	*/

	for (unsigned int i = 0; i < data_size; ++i){
		data[i] = data[i] >> level;
		data[i] = data[i] << level;
	}
}