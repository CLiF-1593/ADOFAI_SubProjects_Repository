#include <Magick++.h>
#include <iostream>
#include "Painter.h"

using namespace Magick;
using namespace std;

int main(int argc, char **argv) {
	InitializeMagick(*argv);
	Painter painter;
	cout << "[ ADOFAI Painter ]" << endl << endl;
	cout << "이미지를 맵으로 변환해줍니다. (투명한 픽셀은 제외시켜줍니다)" << endl;
	cout << "Developer : CLiF" << endl;
	cout << "Version : v 1.1" << endl << endl;

	cout << "Image Path : ";
	string img;
	cin >> img;

	cout << "Adofai Map Path : ";
	string adofai;
	cin >> adofai;

	cout << "Start Tile Number ( n > 0 ) : ";
	unsigned int tile;
	cin >> tile;

	cout << "Tile Style (Standard, Neon, NeonLight, Basic, Gems) : ";
	string type;
	cin >> type;

	cout << "Setting . . . " << endl;
	painter.SetImageFile(img);
	cout << "Drawing . . . " << endl;
	painter.DrawData();
	cout << "Saving . . . " << endl;
	painter.SaveData(adofai, tile, type);
	painter.Save(adofai);
	cout << "Completed" << endl;
	system("pause");
	return 0;
}