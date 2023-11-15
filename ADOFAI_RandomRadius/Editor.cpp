#include "Editor.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
using namespace std;

void Editor::Open(string adofai_file_location) {
	Json::CharReaderBuilder reader;
	ifstream is(adofai_file_location, ifstream::binary);
	string errorMessage;
	auto bret = Json::parseFromStream(reader, is, &this->root, &errorMessage);

	if (bret == false) {
		cout << "Error to parse JSON file !!!" << " " << errorMessage << endl;
		system("exit");
		return;
	}
}

Json::Value Radius(int floor, double scale) {
	Json::Value rad;
	rad["floor"] = floor;
	rad["eventType"] = "ScaleRadius";
	rad["scale"] = scale * 100.0;
	return rad;
}

Json::Value SetSize(int floor, double size) {
	Json::Value rad;
	rad["floor"] = 0;
	rad["eventType"] = "MoveTrack";
	rad["startTile"][0] = floor;
	rad["startTile"][1] = "Start";
	rad["endTile"][0] = floor;
	rad["endTile"][1] = "Start";
	rad["duration"] = 0;
	rad["scale"][0] = size * 100;
	rad["scale"][1] = size * 100;
	rad["angleOffset"] = 0;
	rad["ease"] = "Linear";
	rad["maxVfxOnly"] = "Disabled";
	rad["eventTag"] = "";
	return rad;
}

Json::Value Pause(int floor, double duration) {
	Json::Value rad;
	rad["floor"] = floor;
	rad["eventType"] = "Pause";
	rad["duration"] = duration;
	rad["countdownTicks"] = 0;
	rad["angleCorrectionDir"] = -1;
	return rad;
}

void Editor::ChangeFile() {
	auto *actions = &this->root["actions"];
	auto* tiles = &this->root["angleData"];
	int size = tiles->size();
	auto path = (this->root["pathData"]).asString();
	double rad = -1;
	if (!size) {
		size = path.size();
		for (int i = 1; i < size; i++) {
			actions->append(Radius(i, rad));
			rad *= -1;
		}
	}
	else {
		for (int i = 1; i < size; i++) {
			actions->append(Radius(i, rad));
			rad *= -1;
		}
	}

}

//void Editor::ChangeFile() {
//	srand((unsigned)time(NULL));
//	auto* actions = &this->root["actions"];
//	auto* tiles = &this->root["angleData"];
//	int size = tiles->size();
//	auto path = (this->root["pathData"]).asString();
//	double prev_rad = 1;
//	if (!size) {
//		size = path.size();
//		for (int i = 1; i < size; i++) {
//			double rad = (double)(rand() % 1000) + 100;
//			rad /= 500.0;
//			actions->append(Radius(i, rad));
//			actions->append(SetSize(i, rad));
//			prev_rad = rad;
//		}
//	}
//	else {
//		for (int i = 1; i < size; i++) {
//			double rad = (double)(rand() % 1000) + 100;
//			rad /= 500.0;
//			actions->append(Radius(i, rad));
//			actions->append(SetSize(i, rad));
//			prev_rad = rad;
//		}
//	}
//
//}

//void Editor::ChangeFile() {
//	srand((unsigned)time(NULL));
//	auto* actions = &this->root["actions"];
//	auto* tiles = &this->root["angleData"];
//	int size = tiles->size();
//	auto path = (this->root["pathData"]).asString();
//	if (!size) {
//		size = path.size();
//		for (int i = 1; i < size; i++) {
//			actions->append(Pause(i, -0.005));
//		}
//	}
//	else {
//		for (int i = 1; i < size; i++) {
//			actions->append(Pause(i, -0.005));
//		}
//	}
//
//}

//""

void Editor::Save(string adofai_file_location) {
	FILE *f;
	fopen_s(&f, adofai_file_location.c_str(), "wb");
	fprintf_s(f, "%s", this->root.toStyledString().c_str());
	fclose(f);
}
