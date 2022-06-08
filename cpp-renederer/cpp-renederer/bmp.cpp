#include "obraz.h"

int main()
{
	int w = 600;
	int h = 600;

	// Obraz to klasa z konstruktorem: 
	// Obraz(szerokość obrazu w px, wysokość obrazu w px, vector<Pixel> o wielkości szerokość * wysokość)
	// Pixel(R,G,B) to structura 3 zmiennych R G B (w tej kolejności) int od 0 do 255: domyślnie Pixel(0,0,0) (czarny);
	
	// Obraz.GetKolor(x,y) zwraca Pixel na koordynatach x i y
	// Obraz.SetKolor(Pixel, x, y) podmienia wartości Pixel na podane na koordynatach x i y
	// Obraz.Import("ścieżka") żeby zaimportować plik bmp, musisz stworzyć najpierw dowolny obiekt Obraz i do niego użyć
	// Obraz.Export("ścieżka") zapisuje obiekt do pliku.
	// Wysokość szerokość i vector same się ustawią. Czasami przy zewnętrznych plikach może obrazek inaczej wyglądać bo nie ma obsługi
	// important color i palety kolorów
	
	// Pixel.r (wartość czerwona pixela)
	// Pixel.g (wartość zielona pixela)
	// Pixel.b (wartość niebieska pixela)
	
	// Obraz.height (wysokość obrazu)
	// Obraz.width (szerokość obrazu)
	// Obraz.v_pixel (vector pixeli)
	
	// Tworzenie obiektu o szerokości w i wysokości h
	Obraz bmp(w, h);
	
	// Pętla iterująca po wszystkich x i y
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			
			// If ustawia dolny prawy róg na kolor fioletowy
			if (x > y) {
				bmp.SetKolor(Pixel(120, 120, 50), x, y);
			}
		}
	}
	
	// Wyrzucam zmieniony bmp do pliku
	bmp.Export("../test1.bmp");

	// Tworze dodatkowy Obraz o nazwie kopia i dowolnej wielkości
	Obraz kopia(200,200);
	
	// Do kopia importuje bmp, który wcześniej stworzyłem
	kopia.Import("test.bmp");
	
	// Pętla iteruje dla wszystkich x i y 
	for (int y = 0; y < kopia.height; y++) {
		for (int x = 0; x < kopia.width; x++) {
			
			// Jeżeli czerwony dla pixela jest 0 (Obraz.GetKolor zwraca Pixel a Pixel.r to czerwień pixela)
			// to ustawiam pixel na różowy
			if (kopia.GetKolor(x,y).r==0) {
				kopia.SetKolor(Pixel(50, 200, 30), x, y);
			}
		}
	}
	// Zapisuje nowy obraz do kopia.bmp
	kopia.Export("kopia.bmp");
}
