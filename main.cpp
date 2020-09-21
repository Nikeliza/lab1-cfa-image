#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Image24.h"


using namespace std;

unsigned char hue_transit(unsigned char L1, unsigned char L2, unsigned char L3, unsigned char V1, unsigned char V3) {
	if ((L1 < L2 && L2 < L3) || (L1 > L2 && L2 > L3)) {
		return V1 + ((V3 - V1) * (L2 - L1)) / (L3 - L1);
	}
	else {
		return (V1 + V3) / 2 + (L2 * 2 - L1 - L3) / 4;
	}
}

void red_pixel_norm(Image24& image, int i, int j) {
	unsigned char dN = abs(image[i][j].rgbtRed - image[i - 2][j].rgbtRed) * 2 + abs(image[i - 1][j].rgbtGreen - image[i + 1][j].rgbtGreen);
	unsigned char dE = abs(image[i][j].rgbtRed - image[i][j + 2].rgbtRed) * 2 + abs(image[i][j - 1].rgbtGreen - image[i][j + 1].rgbtGreen);
	unsigned char dW = abs(image[i][j].rgbtRed - image[i][j - 2].rgbtRed) * 2 + abs(image[i][j - 1].rgbtGreen - image[i][j + 1].rgbtGreen);
	unsigned char dS = abs(image[i][j].rgbtRed - image[i + 2][j].rgbtRed) * 2 + abs(image[i - 1][j].rgbtGreen - image[i + 1][j].rgbtGreen);

	if (dN < dE && dN < dW && dN < dS) {
		image[i][j].rgbtGreen = (image[i - 1][j].rgbtGreen * 3 + image[i + 1][j].rgbtGreen + image[i][j].rgbtRed - image[i - 2][j].rgbtRed) / 4;
	}
	else {
		if (dE < dN && dE < dW && dE < dS) {
			image[i][j].rgbtGreen = (image[i][j + 1].rgbtGreen * 3 + image[i][j - 1].rgbtGreen + image[i][j].rgbtRed - image[i][j + 2].rgbtRed) / 4;
		}
		else {
			if (dW < dE && dW < dN && dW < dS) {
				image[i][j].rgbtGreen = (image[i][j - 1].rgbtGreen * 3 + image[i][j + 1].rgbtGreen + image[i][j].rgbtRed - image[i][j - 2].rgbtRed) / 4;
			}
			else {
				image[i][j].rgbtGreen = (image[i + 1][j].rgbtGreen * 3 + image[i - 1][j].rgbtGreen + image[i][j].rgbtRed - image[i + 2][j].rgbtRed) / 4;

			}
		}
	}
	
}

void blue_pixel_norm(Image24& image, int i, int j) {
	unsigned char dN = abs(image[i][j].rgbtBlue - image[i - 2][j].rgbtBlue) * 2 + abs(image[i - 1][j].rgbtGreen - image[i + 1][j].rgbtGreen);
	unsigned char dE = abs(image[i][j].rgbtBlue - image[i][j + 2].rgbtBlue) * 2 + abs(image[i][j - 1].rgbtGreen - image[i][j + 1].rgbtGreen);
	unsigned char dW = abs(image[i][j].rgbtBlue - image[i][j - 2].rgbtBlue) * 2 + abs(image[i][j - 1].rgbtGreen - image[i][j + 1].rgbtGreen);
	unsigned char dS = abs(image[i][j].rgbtBlue - image[i + 2][j].rgbtBlue) * 2 + abs(image[i - 1][j].rgbtGreen - image[i + 1][j].rgbtGreen);

	if (dN < dE && dN < dW && dN < dS) {
		image[i][j].rgbtGreen = (image[i - 1][j].rgbtGreen * 3 + image[i + 1][j].rgbtGreen + image[i][j].rgbtBlue - image[i - 2][j].rgbtBlue) / 4;
	}
	else {
		if (dE < dN && dE < dW && dE < dS) {
			image[i][j].rgbtGreen = (image[i][j + 1].rgbtGreen * 3 + image[i][j - 1].rgbtGreen + image[i][j].rgbtBlue - image[i][j + 2].rgbtBlue) / 4;
		}
		else {
			if (dW < dE && dW < dN && dW < dS) {
				image[i][j].rgbtGreen = (image[i][j - 1].rgbtGreen * 3 + image[i][j + 1].rgbtGreen + image[i][j].rgbtBlue - image[i][j - 2].rgbtBlue) / 4;
			}
			else {
				image[i][j].rgbtGreen = (image[i + 1][j].rgbtGreen * 3 + image[i - 1][j].rgbtGreen + image[i][j].rgbtBlue - image[i + 2][j].rgbtBlue) / 4;

			}
		}
	}
	
}

void red_pixel_blue(Image24& image, int i, int j) {
	unsigned char dNE = abs(image[i - 1][j + 1].rgbtBlue - image[i + 1][j - 1].rgbtBlue) + abs(image[i - 2][j + 2].rgbtRed - image[i][j].rgbtRed) + abs(image[i][j].rgbtRed - image[i + 2][j - 2].rgbtRed) + abs(image[i - 1][j + 1].rgbtGreen - image[i][j].rgbtGreen) + abs(image[i][j].rgbtGreen - image[i + 1][j - 1].rgbtGreen);
	unsigned char dNW = abs(image[i - 1][j - 1].rgbtBlue - image[i + 1][j + 1].rgbtBlue) + abs(image[i - 2][j - 2].rgbtRed - image[i][j].rgbtRed) + abs(image[i][j].rgbtRed - image[i + 2][j + 2].rgbtRed) + abs(image[i - 1][j - 1].rgbtGreen - image[i][j].rgbtGreen) + abs(image[i][j].rgbtGreen - image[i + 1][j + 1].rgbtGreen);
	if (dNE < dNW) {
		image[i][j].rgbtBlue = hue_transit(image[i - 1][j + 1].rgbtGreen, image[i][j].rgbtGreen, image[i + 1][j - 1].rgbtGreen, image[i - 1][j + 1].rgbtBlue, image[i + 1][j - 1].rgbtBlue);
	}
	else {
		image[i][j].rgbtBlue = hue_transit(image[i - 1][j - 1].rgbtGreen, image[i][j].rgbtGreen, image[i + 1][j + 1].rgbtGreen, image[i - 1][j - 1].rgbtBlue, image[i + 1][j + 1].rgbtBlue);
	}
}

void blue_pixel_red(Image24& image, int i, int j) {
	unsigned char dNE = abs(image[i - 1][j + 1].rgbtRed - image[i + 1][j - 1].rgbtRed) + abs(image[i - 2][j + 2].rgbtBlue - image[i][j].rgbtBlue) + abs(image[i][j].rgbtBlue - image[i + 2][j - 2].rgbtBlue) + abs(image[i - 1][j + 1].rgbtGreen - image[i][j].rgbtGreen) + abs(image[i][j].rgbtGreen - image[i + 1][j - 1].rgbtGreen);
	unsigned char dNW = abs(image[i - 1][j - 1].rgbtRed - image[i + 1][j + 1].rgbtRed) + abs(image[i - 2][j - 2].rgbtBlue - image[i][j].rgbtBlue) + abs(image[i][j].rgbtBlue - image[i + 2][j + 2].rgbtBlue) + abs(image[i - 1][j - 1].rgbtGreen - image[i][j].rgbtGreen) + abs(image[i][j].rgbtGreen - image[i + 1][j + 1].rgbtGreen);
	if (dNE < dNW) {
		image[i][j].rgbtRed = hue_transit(image[i - 1][j + 1].rgbtGreen, image[i][j].rgbtGreen, image[i + 1][j - 1].rgbtGreen, image[i - 1][j + 1].rgbtRed, image[i + 1][j - 1].rgbtRed);
	}
	else {
		image[i][j].rgbtRed = hue_transit(image[i - 1][j - 1].rgbtGreen, image[i][j].rgbtGreen, image[i + 1][j + 1].rgbtGreen, image[i - 1][j - 1].rgbtRed, image[i + 1][j + 1].rgbtRed);
	}
}

void green_pixel(Image24& image, int i, int j) {
	image[i - 1][j].rgbtBlue = hue_transit(image[i - 1][j - 1].rgbtGreen, image[i - 1][j].rgbtGreen, image[i - 1][j + 1].rgbtGreen, image[i - 1][j - 1].rgbtBlue, image[i - 1][j + 1].rgbtBlue);
	image[i - 1][j].rgbtRed = hue_transit(image[i - 2][j].rgbtGreen, image[i - 1][j].rgbtGreen, image[i][j].rgbtGreen, image[i - 2][j].rgbtRed, image[i][j].rgbtRed);
}

Image24 ppg(Image24 image)
{
	//Image24 new_image(image.get_width(), image.get_height());
	int flag = 0;
	for (int i = 2; i < image.get_height() - 2; i++) {
		for (int j = 2 + flag; j < image.get_width() - 2; j += 2) {
			if (flag == 0) {
				red_pixel_norm(image, i, j);
			}
			else {
				blue_pixel_norm(image, i, j);
			}
		}
		if (flag == 1) {
			flag = 0;
		}
		else {
			flag = 1;
		}
	}
	flag = 0;
	for (int i = 2; i < image.get_height() - 2; i++) {
		for (int j = 2 + flag; j < image.get_width() - 2; j += 2) {
			if (flag == 0) {
				red_pixel_blue(image, i, j);	
			}
			else {
				blue_pixel_red(image, i, j);
			}
		}
		if (flag == 1) {
			flag = 0;
		}
		else {
			flag = 1;
		}
	}
	flag = 0;

	for (int i = 2; i < image.get_height() - 2; i++) {
		for (int j = 2 + flag; j < image.get_width() - 2; j += 2) {
			green_pixel(image, i, j);
		}
		if (flag == 1) {
			flag = 0;
		}
		else {
			flag = 1;
		}
	}
	return image;
}

double get_mse(Image24 img1, Image24 img2) {
	unsigned long long int razn = 0;
	for (int i = 0; i < img1.get_height(); i++) {
		for (int j = 0; j < img1.get_width(); j++) {
			razn += pow((img1.get_ser(i, j) - img2.get_ser(i, j)), 2);
		}
	}
	return razn / (img1.get_width() * img1.get_height());
}

int main()
{
	Image24 cfa_image;
	cfa_image.read_image("1.bmp");
	//cfa_image.add_padding();
	//Image24 real_image;
	//real_image.read_image("Original.bmp");
	//Image24 image = ppg(cfa_image);
	//image.delete_padding();
	cfa_image.print_image("result.bmp");
	//double a = get_mse(real_image, image);
	//cout << "PSNR " << 10 * log10((255.0 * 255.0) / a) << endl;
	return 0;
}

