#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include "json/json.h"
using namespace std;

int action_cnt = 0;
#define PI 3.14159265

Json::Value Camera(int floor, int angle, string relative) {
	Json::Value camera;
	camera["floor"] = floor;
	camera["eventType"] = "MoveCamera";
	camera["duration"] = 0;
	camera["relativeTo"] = relative;
	camera["position"][0] = -0.5;
	camera["position"][1] = (double)angle / 2.0 / PI;
	camera["rotation"] = 0;
	camera["zoom"] = ((double)angle / 2.0 / PI) * 50 + 100;
	camera["angleOffset"] = 0;
	camera["ease"] = "Linear";
	camera["eventTag"] = "";
	return camera;
}

Json::Value BPM(int floor, double bpm) {
	Json::Value bpm_change;
	bpm_change["floor"] = floor;
	bpm_change["eventType"] = "SetSpeed";
	bpm_change["speedType"] = "Bpm";
	bpm_change["beatsPerMinute"] = bpm;
	bpm_change["bpmMultiplier"] = 1;
	return bpm_change;
}

Json::Value Text(int floor, string t, int size, int angle) {
	Json::Value text;
	text["floor"] = floor;
	text["eventType"] = "AddText"; 
	text["decText"] = t; 
	text["font"] = "Default"; 
	text["position"][0] = 0.5; 
	text["position"][1] = (double)angle / 2.0 / PI;
	text["relativeTo"] = "Tile"; 
	text["pivotOffset"][0] = 0; 
	text["pivotOffset"][1] = 0; 
	text["rotation"] = 0; 
	text["scale"][0] = size;
	text["scale"][1] = size;
	text["color"] = "ffffff"; 
	text["opacity"] = 100;
	text["depth"] = -1; 
	text["parallax"][0] = -1; 
	text["parallax"][1] = -1; 
	text["tag"] = "";
	return text;
}

void process(Json::Value &root, int angle, int floor) {
	root["actions"][action_cnt++] = Text(floor, to_string((int)(angle)), angle * 10 + 100, angle);
	//root["actions"][action_cnt++] = BPM(floor + 1, sqrt(angle) * log(angle) * 30 + 100);
	root["actions"][action_cnt++] = Camera(floor + 1, angle, "Tile");
	for (int i = 0; i < angle; i++) {
		root["angleData"][floor + i] = (double)i / (double)angle * 360.0;
	}
	if (angle != 2022) {
		for (int i = angle; i < angle + angle / 2 + 3; i++) {
			root["angleData"][floor + i] = 0;
		}
	}
}

string get() {
	Json::CharReaderBuilder reader;
	Json::Value root;
	ifstream is("D:\\Game\\ADOFAI\\Content\\Circle\\level.adofai", ifstream::binary);
	string errorMessage;
	auto bret = Json::parseFromStream(reader, is, &root, &errorMessage);
	if (bret == false) {
		cout << "Error to parse JSON file !!!" << " " << errorMessage << endl;
		return "";
	}

	root["angleData"][0] = 0;


	double start_bpm = 100.0;
	root["settings"]["bpm"] = start_bpm;

	root["actions"].clear();
	root["angleData"].clear();

	int floor = 1;

	/*double bpm_change = 10.0;
	for (int i = 3; i < 20; i++) {
		process(root, i, floor);
		floor += i + i / 2 + 3;
	}
	for (int i = 20; i < 100; i += 10) {
		process(root, i, floor);
		floor += i + i / 2 + 3;
	}
	for (int i = 100; i < 200; i += 20) {
		process(root, i, floor);
		floor += i + i / 2 + 3;
	}
	for (int i = 200; i <= 500; i += 50) {
		process(root, i, floor);
		floor += i + i / 2 + 3;
	}
	process(root, 1000, floor);
	floor += 1000 + 500 + 3;
	process(root, 2022, floor);*/

	process(root, 10000, 1);
	return root.toStyledString();
}

int main() {
	
	string str = get();
	if (str.empty())
		return 1;

	FILE *f;
	fopen_s(&f, "D:\\Game\\ADOFAI\\Content\\Circle\\level.adofai", "wb");
	fprintf_s(f, "%s", str.c_str());
	fclose(f);
	return 0;
}