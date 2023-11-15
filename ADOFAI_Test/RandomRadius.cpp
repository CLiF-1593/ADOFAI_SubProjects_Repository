#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include "json/json.h"
using namespace std;

int action_cnt = 0;
#define PI 3.14159265

Json::Value Radius(int floor, double scale) {
	Json::Value rad;
	rad["floor"] = floor;
	rad["eventType"] = "ScaleRadius";
	rad["scale"] = scale * 100.0;
	return rad;
}

void process(Json::Value& root, double angle, int floor) {
	root["angleData"][floor] = 180.0 - angle;
	root["angleData"][floor + 1] = 999;
	root["angleData"][floor + 2] = 0;
}

string get() {
	Json::CharReaderBuilder reader;
	Json::Value root;
	ifstream is("D:\\Game\\ADOFAI\\Content\\N_Pseudo\\level.adofai", ifstream::binary);
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
	int action_cnt = 0;

	int floor = 2;

	for (int k = 1; k <= 10; k++) {
		root["actions"][action_cnt++] = Text(floor, to_string(k), 100, 0, 1);
		for (int i = 1; i <= k; i++) {
			process(root, 30.0 * (double)i / (double)k, floor);
			floor += 2;
		}
		floor += 2;
	}
	root["actions"][action_cnt++] = Text(floor, "100", 100, 0, 1);
	for (int i = 1; i <= 100; i++) {
		process(root, 30.0 * (double)i / 100.0, floor);
		floor += 2;
	}
	floor += 2;
	root["actions"][action_cnt++] = Text(floor, "1000", 100, 0, 1);
	for (int i = 1; i <= 1000; i++) {
		process(root, 30.0 * (double)i / 1000.0, floor);
		floor += 2;
	}
	floor += 2;
	return root.toStyledString();
}

int main() {

	string str = get();
	if (str.empty())
		return 1;

	FILE* f;
	fopen_s(&f, "D:\\Game\\ADOFAI\\Content\\N_Pseudo\\level.adofai", "wb");
	fprintf_s(f, "%s", str.c_str());
	fclose(f);
	return 0;
}