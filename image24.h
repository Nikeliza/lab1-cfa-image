#pragma once
#include <vector>
#include "Windows.h";

class Image24
{
	std::vector<std::vector<RGBTRIPLE>> image;
	BITMAPFILEHEADER bfh_l;
	BITMAPINFOHEADER bih_l;

public:
	Image24(int width, int height);
	Image24();
	void read_image(const char* file_name);
	void print_image(const char* file_name);
	void add_padding();
	void delete_padding();
	int get_height();
	int get_width();
	int get_ser(int i, int j);
	std::vector<RGBTRIPLE>& operator[] (const int index);
};

