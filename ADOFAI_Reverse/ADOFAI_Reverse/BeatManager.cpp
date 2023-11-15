#include "BeatManager.h"
#include <iostream>

#define CurrentTile(x, y) (!x.empty() && x.front().tile_num == y)

double SetAngleValue(double angle) {
	if (angle != 999) {
		while (angle > 360) {
			angle -= 360;
		}
		while (angle <= 0) {
			angle += 360;
		}
	}
	return angle;
}

void BeatManager::Init(FileManager &file) {
	auto angle = file.GetAngleData();
	double cur_bpm = file.GetBPM();

	auto bpm = file.GetBPMList();
	auto twirl = file.GetTwirlList();
	auto planets = file.GetPlanetList();
	auto pause = file.GetPauseList();
	auto roam = file.GetFreeRoamList();

	this->beat_list.clear();
	int tile_num = 0;

	bool reverse = false;
	double planet_angle = 0;
	bool planet_angle_reverse = false;
	double prev_angle = 180;

	while(!angle.empty()) {
		if (CurrentTile(bpm, tile_num)) {
			cur_bpm = bpm.front().value;
			bpm.pop();
		}
		if (CurrentTile(twirl, tile_num)) {
			reverse = !reverse;
			twirl.pop();
		}
		if (CurrentTile(planets, tile_num)) {
			planet_angle = (180 * (double)(planets.front().value - 2) / (double)planets.front().value);
			planets.pop();
		}

		auto cur_angle = SetAngleValue(angle.front());
		angle.pop();

		
		if (cur_angle == 999) {
			prev_angle = SetAngleValue(prev_angle - 180);
			planet_angle_reverse = true;
			tile_num++;
			continue;
		}
		
		double delta_angle = SetAngleValue((prev_angle - cur_angle) * (1 - reverse * 2) - planet_angle * (!planet_angle_reverse));

		Beat beat;
		beat.beat = delta_angle / 180.0;
		beat.bpm = cur_bpm;
		beat.hold = NONE;
		beat.autoplay = false;
		this->beat_list.push_back(beat);

		if (CurrentTile(pause, tile_num)) {
			this->beat_list.back().beat += pause.front().value;
			pause.pop();
		}
		if (CurrentTile(roam, tile_num)) {
			this->beat_list.back().beat = roam.front().value;
			roam.pop();
		}
		
		planet_angle_reverse = false;
		prev_angle = SetAngleValue(cur_angle + 180);
		tile_num++;
	}
	return;
}

vector<Beat> BeatManager::GetBeatList() {
	return this->beat_list;
}

void BeatManager::Log() {
	cout << "[Log] BeatManager" << endl;
	cout << "beat_list (type=vector<Beat>)" << endl;
	cout << "data = ";
	cout << "["<< endl;
	for (int i = 0; i < this->beat_list.size(); i++) {
		cout << "\t(" << beat_list[i].beat << ", " << beat_list[i].bpm << ", " << beat_list[i].hold << ")";
		if (i != this->beat_list.size() - 1) {
			cout << ", ";
		}
		cout << endl;
	}
	cout << "]";
	cout << endl;
}
