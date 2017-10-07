/**
 * @file img.h
 *
 * @brief img class definition
 *
 * Class that representing the real bmp file in real world.
 * Providing basic read, wirte and advance function such as
 * quantization and bilinear resize.
 *
 */

#ifndef IMG_H_
#define IMG_H_

#include <string>

using namespace std;

class img{

public:

	img();					//	img constructed by this constructor will be empty at first
	img(string in_name);	//	img constructed by this constructor will open the image correspond to in_name
	~img();					//	distructor

	bool loaded;								//	indicator shows img is empty or not
	void imread(string in_name);				//	image reading
	void imwrie(string out_name);				//	image export to file
	void bilinear(img& store, bool is_up);		//	bilinear resize
	void quant(int level);						//	qunatization

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

#endif