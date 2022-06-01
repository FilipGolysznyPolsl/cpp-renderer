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
}
