#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <Magick++.h>
#include <iostream>
#include "json/json.h"

using namespace Magick;
using namespace std;

struct RGBA {
	int r;
	int g;
	int b;
	int a;
};

class Painter {
private:
	vector<pair<pair<int, int>, RGBA>> data;
	string img_path;
	Image img;
	int width, height;

	void Drawing(int x, int y);
	ColorRGB GetColor(int x, int y);
	bool Filled(int x, int y) {
		return this->GetColor(x, y).alpha() != 0;
	}

	string adofai_dat;

public:
	void SetImageFile(string img_path);
	void DrawData();
	void SaveData(string adofai_path, int position, string mode);
	void Save(string adofai_path);
};

