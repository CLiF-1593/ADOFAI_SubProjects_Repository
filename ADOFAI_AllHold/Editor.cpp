#include "Editor.h"
#include <iostream>
#include <fstream>
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

Json::Value Hold(int floor) {
	Json::Value hold;
	hold["floor"] = floor;
	hold["eventType"] = "Hold";
	hold["duration"] = 0;
	hold["distanceMultiplier"] = 100;
	hold["landingAnimation"] = "Enabled";
	return hold;
}

void Editor::ChangeFile() {
	auto *actions = &this->root["actions"];
	if (this->root.isMember("angleData")) {
		auto* tiles = &this->root["angleData"];
		int size = tiles->size();
		for (int i = 1; i < size - 1; i++) {
			if (tiles[i] == 999 or tiles[i - 1] == 999)
				continue;
			actions->append(Hold(i));
			//i++;
		}
	}
	else {
		auto path = (this->root["pathData"]).asString();
		int size = path.size();
		for (int i = 1; i < size - 1; i++) {
			if (path[i] == '!' or path[i - 1] == '!')
				continue;
			actions->append(Hold(i));
			//i++;
		}
	}
}

void Editor::Save(string adofai_file_location) {
	FILE *f;
	fopen_s(&f, adofai_file_location.c_str(), "wb");
	fprintf_s(f, "%s", this->root.toStyledString().c_str());
	fclose(f);
}
