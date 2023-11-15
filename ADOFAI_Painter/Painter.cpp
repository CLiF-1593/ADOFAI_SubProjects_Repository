#include "Painter.h"
#include <fstream>

void Painter::Drawing(int x, int y) {
	//Coordinate Check
	if (x < 0 || y < 0 || x >= this->width || y >= this->height)
		return;

	//Transparent Check
	if (!this->Filled(x, y))
		return;

	//Data Push
	pair<pair<int, int>, RGBA> dat;
	dat.first.first = x;
	dat.first.second = y;
	auto pixel = this->GetColor(x, y);
	dat.second.r = pixel.red() * 255.0;
	dat.second.g = pixel.green() * 255.0;
	dat.second.b = pixel.blue() * 255.0;
	dat.second.a = pixel.alpha() * 255.0;
	this->data.push_back(dat);

	//Alpha Set
	pixel.alpha(0);
	pixel.red(0);
	pixel.green(0);
	pixel.blue(0);
	this->img.pixelColor(x, y, pixel);

	//Search
	Drawing(x, y - 1); //up
	Drawing(x - 1, y); //left
	Drawing(x - 1, y - 1); //left up
	Drawing(x + 1, y); //right
	Drawing(x + 1, y - 1); //right up
	Drawing(x, y + 1); //down
	Drawing(x - 1, y + 1); //left down
	Drawing(x + 1, y + 1); //right down
	
}

ColorRGB Painter::GetColor(int x, int y) {
	ColorRGB rgb(img.pixelColor(x, y));
	return rgb;
}

void Painter::SetImageFile(string img_path) {
	this->img_path = img_path;
	this->data.clear();
}

void Painter::DrawData() {
	this->img.read(this->img_path);
	this->width = img.size().width();
	this->height = img.size().height();
	for (int i = 0; i < this->width; i++) {
		for (int j = 0; j < this->height; j++) {
			this->Drawing(i, j);
		}
	}
}

Json::Value Position(int floor, int x, int y) {
	Json::Value pos;
	pos["floor"] = floor;
	pos["eventType"] = "PositionTrack";
	pos["positionOffset"][0] = x;
	pos["positionOffset"][1] = y;
	pos["editorOnly"] = "Disabled";
	return pos;
}

string debug_string(string str) {
	while (str.size() < 2) {
		str = "0" + str;
	}
	return str;
}

Json::Value SetColor(int floor, RGBA color, string mode) {
	Json::Value c;
	c["floor"] = floor;
	c["eventType"] = "ColorTrack";
	c["trackColorType"] = "Single";
	std::stringstream rs, gs, bs, as;
	rs << std::hex << color.r;
	gs << std::hex << color.g;
	bs << std::hex << color.b;
	as << std::hex << color.a;
	string result(debug_string(rs.str()) + debug_string(gs.str()) + debug_string(bs.str()) + debug_string(as.str()));
	c["trackColor"] = result;
	c["secondaryTrackColor"] = "ffffff";
	c["trackColorAnimDuration"] = 2;
	c["trackColorPulse"] = "None";
	c["trackPulseLength"] = 10;
	c["trackStyle"] = mode;
	c["trackTexture"] = "";
	c["trackTextureScale"] = 1;
	return c;
}

void Painter::SaveData(string adofai_path, int position, string mode) {
	Json::CharReaderBuilder reader;
	Json::Value root;
	ifstream is(adofai_path, ifstream::binary);
	string errorMessage;
	auto bret = Json::parseFromStream(reader, is, &root, &errorMessage);

	if (bret == false) {
		cout << "Error to parse JSON file !!!" << " " << errorMessage << endl;
		return;
	}

	auto *actions = &root["actions"];
	auto *angle_data = &root["angleData"];

	int x = 0, y = 0;

	Json::Value camera;
	camera["floor"] = position - 1;
	camera["eventType"]= "MoveCamera";
	camera["duration"] = 0;
	camera["relativeTo"] = "Tile";
	camera["position"][0] = this->width / 2;
	camera["position"][1] = -this->height / 2;
	camera["rotation"] = 0;
	camera["zoom"] = 15 * (this->width > this->height ? this->width : this->height);
	camera["angleOffset"] = 0;
	camera["ease"] = "Linear";
	camera["eventTag"] = "";
	actions->append(camera);

	for (int i = 0; i < this->data.size(); i++) {
		angle_data->insert(position, 0); //angle insert
		x = this->data[i].first.first - x - 1;
		y = y - this->data[i].first.second;
		actions->append(Position(i + position, x, y));
		actions->append(SetColor(i + position, this->data[i].second, mode));
		x = this->data[i].first.first;
		y = this->data[i].first.second;
	}

	this->adofai_dat = root.toStyledString();
}

void Painter::Save(string adofai_path) {
	FILE *f;
	fopen_s(&f, adofai_path.c_str(), "wb");
	fprintf_s(f, "%s", this->adofai_dat.c_str());
	fclose(f);
}
