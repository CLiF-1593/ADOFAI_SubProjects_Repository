#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include "json/json.h"
using namespace std;

int action_cnt = 0;
int deco_cnt = 0;
#define PI 3.14159265

Json::Value Camera(int floor, string relative) {
	Json::Value camera;
	camera["floor"] = floor;
	camera["eventType"] = "MoveCamera";
	camera["duration"] = 0;
	camera["relativeTo"] = relative;
	camera["position"][0] = 0;
	camera["position"][1] = 0;
	camera["angleOffset"] = 0;
	camera["ease"] = "Linear";
	camera["eventTag"] = "";
	camera["dontDisable"] = "Disabled";
	camera["minVfxOnly"] = "Disabled";
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

Json::Value Pause(int floor, double duration) {
	Json::Value pause;
	pause["floor"] = floor;
	pause["eventType"] = "Pause";
	pause["duration"] = duration;
	pause["countdownTicks"] = 0;
	pause["angleCorrectionDir"] = 0;
	return pause;
}

Json::Value Position(int floor) {
	Json::Value pause;
	pause["floor"] = floor;
	pause["eventType"] = "PositionTrack";
	pause["positionOffset"][0] = 10;
	pause["positionOffset"][1] = 0;
	pause["rotation"] = 0;
	pause["scale"] = 100;
	pause["opacity"] = 100;
	pause["editorOnly"] = "Disabled";
	return pause;
}

Json::Value Text(int floor, string t, int size, double x, double y) {
	Json::Value text;
	text["floor"] = floor;
	text["eventType"] = "AddText";
	text["decText"] = t;
	text["font"] = "Default";
	text["position"][0] = x;
	text["position"][1] = y;
	text["relativeTo"] = "Tile";
	text["pivotOffset"][0] = 0;
	text["pivotOffset"][1] = 0;
	text["rotation"] = 0;
	text["scale"][0] = size;
	text["scale"][1] = size;
	text["color"] = "DDDDDD";
	text["opacity"] = 100;
	text["depth"] = -1;
	text["parallax"][0] = -1;
	text["parallax"][1] = -1;
	text["tag"] = "";
	return text;
}

void process(Json::Value& root, double bpm) {
	root["angleData"].append(0);
	//root["actions"][action_cnt++] = Text(floor, to_string((int)(angle)), angle * 10 + 100, angle);
	root["actions"][action_cnt++] = BPM(root["angleData"].size() , bpm);
	root["actions"][action_cnt++] = Pause(root["angleData"].size(), bpm / 100);
	root["actions"][action_cnt++] = Camera(root["angleData"].size(), "Tile");
	root["actions"][action_cnt++] = Position(root["angleData"].size());
	//root["decorations"][deco_cnt++] = Text(root["decorations"].size() + 1, "Angular Velocity = " + to_string(bpm / 60.0) + "¥ð (rad/s)", 35, -1.5, 1.35);
}

void get() {
	Json::CharReaderBuilder reader;
	Json::Value root;
	ifstream is("D:\\Game\\ADOFAI\\Content\\RotationSpeed\\level.adofai", ifstream::binary);
	string errorMessage;
	auto bret = Json::parseFromStream(reader, is, &root, &errorMessage);
	if (bret == false) {
		cout << "Error to parse JSON file !!!" << " " << errorMessage << endl;
		return;
	}

	root["angleData"][0] = 0;


	double start_bpm = 100.0;
	root["settings"]["bpm"] = start_bpm;

	root["actions"].clear();
	root["angleData"].clear();
	int action_cnt = 0;

	int t = 360;
	for (int i = 0; i < 200; i++) {
		//cout << t << " " << (double)t / 60.0 << endl;
		process(root, t);
		t += 10;
	}
	//process(root, 360.0 + 50.0 * 10.0);
	//process(root, 360.0 + 50.0 * 11.55);
	//process(root, 360.0 + 50.0 * 15.29);
	//process(root, 360.0 + 50.0 * 20.93);
	////process(root, 360.0 + 50.0 * 26.0);
	//process(root, 360.0 + 50.0 * 30.28);
	//process(root, 360.0 + 50.0 * 37.85);
	//process(root, 360.0 * 16);

	ofstream json_file;
	json_file.open("D:\\Game\\ADOFAI\\Content\\RotationSpeed\\level.adofai");
	Json::StreamWriterBuilder builder;
	//builder.settings_["emitUTF8"] = true;
	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	writer->write(root, &json_file);
	json_file.close();
}

int main() {
	get();
	return 0;
}