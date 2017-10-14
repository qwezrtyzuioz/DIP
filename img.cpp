/**
* @ file img.cpp
*
* @ member function definition of img class
*
* Define constructor, distructor.
* Define function that import external image into img class.
* Define function that export img to external image.
* Define function that create img with another size
* Define function that quantize img data inside every pixel.
*
*/

#include "img.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>



// ------------------------------------------------------------------------------
//   Constructor
// ------------------------------------------------------------------------------
/*
* Default constructor
* Note that img class created by this constructor will be empty initially.
*/

img::img()
	:loaded(false){}	//	Note the image is empty

// ------------------------------------------------------------------------------
//   Constructor
// ------------------------------------------------------------------------------
/*
* Constructor that will import external image to img class.
*/

img::img(string in_name)
	: loaded(true)
{
	// ------------------------------------------------------------------------------
	//   File reading
	// ------------------------------------------------------------------------------
	imread(in_name);
}

// ------------------------------------------------------------------------------
//   Destructor
// ------------------------------------------------------------------------------
/*
* Free the memory that was dynamic allocated.
*/

img::~img()
{
	// ------------------------------------------------------------------------------
	//   Data Deloaction
	// ------------------------------------------------------------------------------

	if (loaded)
		delete[] data;
}

// ------------------------------------------------------------------------------
//   Image reading
// ------------------------------------------------------------------------------
/*
* Check file_name exist or not.
* Analsis the real size of file.
* Read header and data of bmp.
*/

void img::imread(string in_name)
{

	// ------------------------------------------------------------------------------
	//   File opening
	// ------------------------------------------------------------------------------

	ifstream fin;
	fin.open(in_name.c_str(), ios::in | ios::binary);

	if (!fin.is_open()){
		cout << "ERROR!! Image Doesn't Exist Can't be Loaded!! " << endl;
		return;
	}


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

// ------------------------------------------------------------------------------
//   Image writting
// ------------------------------------------------------------------------------
/*
* Check file_name exist or not.
* Write header and data of bmp.
*/

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
	fout.open(out_name.c_str(), ios::out | ios::binary);

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

// ------------------------------------------------------------------------------
//   Bilinear resize
// ------------------------------------------------------------------------------
/*
* 1. Check orgin and output img is illegal or not
* 2. Envalue the target size of new image
* 3. Fill the header of new image base on target size
* 4. Declare parameters should be used
* 5. For each pixel on new image
*		1. Envalue the corresponding point
*		2. Interpolate once make r1, r2
*		3. Interpolate twice make final value
*		4. Write final value back to new image
*/

void img::bilinear(img& store, bool is_up)
{
	int	new_width, new_height;	//	width and height after resize

	// ------------------------------------------------------------------------------
	//   Checking
	// ------------------------------------------------------------------------------

	if (!loaded){
		cout << "ERROR!! Empty Image Can't be Resize!!" << endl;
		return;
	}
	if (store.loaded){
		cout << "ERROR!! The output object is not empty" << endl;
		return;
	}

	// ------------------------------------------------------------------------------
	//	Envalue the destination
	// ------------------------------------------------------------------------------

	if (is_up){
		new_width = int(width* 1.5);
		new_height = int(height* 1.5);
		new_width = (new_width / 4) * 4;
		//	Note that a width of a bmp image must be the multiple of 4.
	}
	else{
		new_width = int(width / 1.5);
		new_height = int(height / 1.5);
		new_width = (new_width / 4) * 4;
		//	Note that a width of a bmp image must be the multiple of 4.
	}

	// ------------------------------------------------------------------------------
	//	Header filling
	// ------------------------------------------------------------------------------

	//	these parameters in resized image the same as origin one.
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
	//	these parameter in resized image change as size.
	store.width = new_width;
	store.height = new_height;
	store.data_size = (store.width* store.height* store.pix_bit) / 8;
	store.file_size = store.data_size + store.offset;
	store.loaded = true;
	store.real_size = store.file_size;
	store.data = new char[store.data_size + 2 * store.pix_bit* store.width / 8];

	// ------------------------------------------------------------------------------
	//	Parameters declaration
	// ------------------------------------------------------------------------------

	int
		step,						//	shows how many byte express a pixel
		r1[4], r2[4], inp[4],		//	r1 and r2 are values caculated after first interpolation
		qx, qy;						//	the down left integer point

	unsigned int
		pos, pos1, pos2;			//	used to store the real position convert from a coordinate to data array

	double
		a, b,						//	distance from the point we want to interpolate to both reference points
		cor_x, cor_y;				//	corresponding point form pixel on resized image mapping to original one

	step = pix_bit / 8;

	// ------------------------------------------------------------------------------
	//	Interpolation
	// ------------------------------------------------------------------------------

	for (int y = 0; y < new_height; ++y){	//	for each pixel on resized image
		for (int x = 0; x < new_width; ++x){

			//	envalue the position of corresponding point.
			if (is_up){
				cor_x = (double)x / 1.5;
				cor_y = (double)y / 1.5;
			}
			else{
				cor_x = (double)x * 1.5;
				cor_y = (double)y * 1.5;
			}

			//	envalue the corresponding points
			qx = (int)cor_x;
			qy = (int)cor_y;

			//	envalue the real position of reference points to r1
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

			//	caculate the ratio of distance from corresponding point to both reference points r1 r2
			a = abs(cor_y - qy);
			b = abs(1 - a);
			//	intopolate the corresponding point according to the r1 r2 and the ratio
			for (int i = 0; i < step; ++i){
				inp[i] = a* (double)r2[i] + b* (double)r1[i];
			}

			//	Write back to output object
			for (int i = 0; i < step; ++i){
				pos = (x + y* store.width) * step;
				store.data[pos + i] = inp[i];
				if (inp[i]<0) cout << inp[i] << endl;
			}

		}
	}

}

// ------------------------------------------------------------------------------
//   Quantize Function
// ------------------------------------------------------------------------------
/*
* Shift the data store in bmp to right then to lefe. This operation will
* discard unsignificant bit and turn them to zero.
* The level is the compress ratio. 1 is no Compress, 7 is Thresholding.
*/

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
	//   Quantization
	// ------------------------------------------------------------------------------

	for (unsigned int i = 0; i < data_size; ++i){
		data[i] = data[i] >> level;
		data[i] = data[i] << level;
	}
}