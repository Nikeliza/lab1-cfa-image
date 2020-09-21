#include "Image24.h"
#include <fstream>
#include <iostream>
//#include "stdafx.h"
//#include<iostream>


Image24::Image24(int width, int height)
{
	image = std::vector<std::vector<RGBTRIPLE>>(height);
	for (int i = 0; i < height; i++) {
		image[i] = std::vector<RGBTRIPLE>(width);
	}
}

Image24::Image24()
{
}

void Image24::read_image(const char* file_name)
{
	try {
		// ќткрываем файл
		std::ifstream file(file_name, std::ios::in | std::ios::binary);
		if (!file.is_open())
		{
			std::cout << "file not open" << std::endl;
			return;
		}
		int size, pixels_adress, width, height;
		short int bits_per_pixel;

		file.read((char*)&bfh_l, sizeof(bfh_l));
		file.read((char*)&bih_l, sizeof(bih_l));
		
		image = std::vector<std::vector<RGBTRIPLE>>(bih_l.biHeight);

		for (int x = bih_l.biHeight - 1; x >= 0; x--)
		{
			std::vector<RGBTRIPLE> mas1(bih_l.biWidth);
			for (int y = 0; y < bih_l.biWidth; y++)
			{
				file.read((char*)&mas1[y], 3);

			}
			image[x] = mas1;
		}
		
		file.close();
	}
	catch(const std::exception& e){
		std::cout << e.what() << std::endl;
	}
	
}

void Image24::print_image(const char* file_name)
{
	try {
		// ќткрываем файл
		std::ofstream file(file_name, std::ios::out | std::ios::binary);
		if (!file.is_open())
		{
			std::cout << "file not open for write" << std::endl;
			return;
		}
		int size, pixels_adress, width, height;
		short int bits_per_pixel;

		file.write((char*)&bfh_l, sizeof(bfh_l));
		file.write((char*)&bih_l, sizeof(bih_l));

		unsigned int bgr = 0;
		size_t padding = 0;
		if ((bih_l.biWidth * 3) % 4) padding = 4 - (bih_l.biWidth * 3) % 4;
		for (int x = bih_l.biHeight - 1; x >= 0; x--)
		{

			for (int y = 0; y < bih_l.biWidth; y++)
			{
				file.write((char*)&image[x][y], 3);
			}
			for (int y = 0; y < padding; y++) {
				file.write((char*)&image[x][bih_l.biWidth - 1], 3);
			}

		}
		file.close();
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	
}

void Image24::add_padding()
{
	std::vector<RGBTRIPLE> line1(image[get_height() - 2]);
	std::vector<RGBTRIPLE> line2(image[get_height() - 3]);
	std::vector<RGBTRIPLE> line3(image[1]);
	std::vector<RGBTRIPLE> line4(image[2]);
	
	image.push_back(line1);
	image.push_back(line2);
	auto beg = image.begin();
	image.insert(beg, line3);
	beg = image.begin();
	image.insert(beg, line4);
	int len = get_width();
	for (int i = 0; i < get_height(); i++) {
		RGBTRIPLE el1 = image[i][len - 2];
		RGBTRIPLE el2 = image[i][len - 3];
		RGBTRIPLE el3 = image[i][1];
		RGBTRIPLE el4 = image[i][2];
		
		image[i].push_back(el1);
		image[i].push_back(el2);
		auto beg = image[i].begin();
		image[i].insert(beg, el3);
		beg = image[i].begin();
		image[i].insert(beg, el4);
	}
	int a = 0;
}

void Image24::delete_padding()
{
	image.erase(image.begin());
	image.erase(image.begin());
	image.pop_back();
	image.pop_back();

	for (int i = 0; i < get_height(); i++) {
		
		image[i].erase(image[i].begin());
		image[i].erase(image[i].begin());
		image[i].pop_back();
		image[i].pop_back();
	}
}

int Image24::get_height()
{
	return image.size();
}

int Image24::get_width()
{
	return image[0].size();
}

int Image24::get_ser(int i, int j)
{
	return 0.144 * image[i][j].rgbtBlue + 0.587 * image[i][j].rgbtGreen + 0.299 * image[i][j].rgbtRed;
}

std::vector<RGBTRIPLE>& Image24::operator[](const int index)
{
	return image[index];
}
