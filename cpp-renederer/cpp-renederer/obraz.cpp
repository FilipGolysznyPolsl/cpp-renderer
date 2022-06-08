#include "obraz.h"

Pixel::Pixel() : r(0), g(0), b(0)
{
}

Pixel::Pixel(float r, float g, float b) : r(256-r), g(256-g), b(256-b)
{
}

Pixel::~Pixel()
{
}

Obraz::Obraz(int w, int h) : width(w), height(h), v_pixel(vector<Pixel>(w*h))
{
}

Obraz::~Obraz()
{
}

Pixel Obraz::GetKolor(int x, int y) const
{
	return v_pixel[y * width + x];
}

void Obraz::SetKolor(const Pixel& kolor, int x, int y)
{
	v_pixel[y * width + x].r = kolor.r;
	v_pixel[y * width + x].g = kolor.g;
	v_pixel[y * width + x].b = kolor.b;
}

void Obraz::Export(const char* path)
{
	const int HeaderL = 14;
	const int InfHeadL = 40;

	char padding[3] = {0, 0, 0};
	const int paddingL = ((4 - (width * 3) % 4) % 4);

	const int ContentL = HeaderL + InfHeadL + width * height * 3 + paddingL * width;

	unsigned char Header[HeaderL];

	// Typ pliku
	Header[0] = 'B';
	Header[1] = 'M';
	// Wielkoœæ pliku
	Header[2] = ContentL;
	Header[3] = ContentL >> 8;
	Header[4] = ContentL >> 16;
	Header[5] = ContentL >> 24;
	// Zarezerwowane
	Header[6] = 0;
	Header[7] = 0;
	Header[8] = 0;
	Header[9] = 0;
	// Przesuniêcie zawartoœci o wielkoœæ nag³ówka
	Header[10] = HeaderL + InfHeadL;
	Header[11] = 0;
	Header[12] = 0;
	Header[13] = 0;


	unsigned char InfHead[InfHeadL];

	// Wielkoœæ nag³ówka
	InfHead[0] = InfHeadL;
	InfHead[1] = 0;
	InfHead[2] = 0;
	InfHead[3] = 0;
	// Szerokoœæ obrazu
	InfHead[4] = width;
	InfHead[5] = width >> 8;
	InfHead[6] = width >> 16;
	InfHead[7] = width >> 24;
	// Wysokoœæ obrazu
	InfHead[8] = height;
	InfHead[9] = height  >> 8;
	InfHead[10] = height >> 16;
	InfHead[11] = height >> 24;
	// Iloœæ warstw
	InfHead[12] = 1;
	InfHead[13] = 0;
	// Rozmiar piksela w bitach
	InfHead[14] = 24;
	InfHead[15] = 0;

	// Reszta jest nam niepotrzebna (zaawansowane zastosowania)
	// Algorytm Kompresji
	InfHead[16] = 0;
	InfHead[17] = 0;
	InfHead[18] = 0;
	InfHead[19] = 0;
	// Wielkoœæ obrazu (do kompresji)
	InfHead[20] = 0;
	InfHead[21] = 0;
	InfHead[22] = 0;
	InfHead[23] = 0;
	// Pixele na metr w osi X
	InfHead[24] = 0;
	InfHead[25] = 0;
	InfHead[26] = 0;
	InfHead[27] = 0;
	// Pixele na metr w osi Y
	InfHead[28] = 0;
	InfHead[29] = 0;
	InfHead[30] = 0;
	InfHead[31] = 0;
	// Palety kolorów
	InfHead[32] = 0;
	InfHead[33] = 0;
	InfHead[34] = 0;
	InfHead[35] = 0;
	// Wa¿ne kolory
	InfHead[36] = 0;
	InfHead[37] = 0;
	InfHead[38] = 0;
	InfHead[39] = 0;

	ofstream file;
	file.open(path, ios::out | ios::binary);
	file.write(reinterpret_cast<char*>(Header), HeaderL);
	file.write(reinterpret_cast<char*>(InfHead), InfHeadL);
	for (int y = 0; y < width; y++) {
		for (int x = 0; x < height; x++) {
			unsigned char r = static_cast<unsigned char>(GetKolor(x, y).r * 255.0f);
			unsigned char g = static_cast<unsigned char>(GetKolor(x, y).g * 255.0f);
			unsigned char b = static_cast<unsigned char>(GetKolor(x, y).b * 255.0f);

			unsigned char color[3] = { b, g, r };

			file.write(reinterpret_cast<char*>(color), 3);
		}
		file.write(reinterpret_cast<char*>(padding), paddingL);
	}
	file.close();
	cout << "Utworzono BMP!\n";
}

void Obraz::Import(const char* path)
{
	ifstream file;
	file.open(path, ios::in | ios::binary);
	if (!file.is_open()) {
		cout << "Nie ma takiego pliku\n";
		return;
	}

	const int HeaderL = 14;
	const int InfHeadL = 40;

	unsigned char Header[HeaderL];
	file.read(reinterpret_cast<char*>(Header), HeaderL);
	if (!(Header[0] == 'B' && Header[1] == 'M')) {
		cout << "To nie plik BMP!\n";
		return;
	}
	unsigned char InfHead[InfHeadL];
	file.read(reinterpret_cast<char*>(InfHead), InfHeadL);

	int ContentL = Header[2] + (Header[3] << 8) + (Header[4] << 16) + (Header[5] << 24);
	width = InfHead[4] + (InfHead[5] << 8) + (InfHead[6] << 16) + (InfHead[7] << 24);
	height = InfHead[8] + (InfHead[9] << 8) + (InfHead[10] << 16) + (InfHead[11] << 24);

	v_pixel.resize(width * height);

	const int paddingL = ((4 - (width * 3) % 4) % 4);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			unsigned char color[3];
			file.read(reinterpret_cast<char*>(color), 3);
			


			v_pixel[y * width + x].r = static_cast<float>(color[2]) / 255.0f;
			v_pixel[y * width + x].g = static_cast<float>(color[1]) / 255.0f;
			v_pixel[y * width + x].b = static_cast<float>(color[0]) / 255.0f;
		}
		file.ignore(paddingL);
	}

	file.close();
	cout << "Zaladowano plik\n";
}

