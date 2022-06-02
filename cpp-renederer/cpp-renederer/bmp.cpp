#include "obraz.h"

int main()
{
	int w = 600;
	int h = 600;

	Obraz bmp(w, h);
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			if (x > y) {
				bmp.SetKolor(Pixel(120, 120, 50), x, y);
			}
		}
	}

	bmp.Export("test.bmp");



	Obraz kopia(200,200);

	kopia.Import("test.bmp");

	for (int y = 0; y < kopia.height; y++) {
		for (int x = 0; x < kopia.width; x++) {
			if (kopia.GetKolor(x,y).r==0) {
				kopia.SetKolor(Pixel(50, 200, 30), x, y);
			}
		}
	}
	kopia.Export("kopia.bmp");
}
