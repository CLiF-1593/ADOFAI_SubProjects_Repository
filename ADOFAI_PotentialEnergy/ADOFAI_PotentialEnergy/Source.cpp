#include <iostream>
#include <string>
#include <Windows.h>
#include <ctime>
#include <vector>
#include "File.h"
#include "EventJson.h"
#include "AngleData.h"
#include "Physics.h"
using namespace std;

#define TILE_LENGTH 2.0

int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	cout << "[ ADOFAI Potential Energy Simulator ]" << endl << endl;
	cout << "Developer : CLiF (https://www.youtube.com/@clif-1593)" << endl;
	cout << "Version : v0.1" << endl << endl;

	cout << "< Choose the ADOFAI File - 얼불춤 파일 선택 >" << endl;
	string adofai_path = OpenFileName(ADOFAI);
	if (adofai_path.empty())
		return 0;
	string folder_path = adofai_path;
	while (folder_path.back() != '\\') {
		folder_path.pop_back();
	}

	Json::Value adofai;
	ReadJson(adofai_path, adofai);

	double bpm_i = adofai["settings"]["bpm"].asDouble();

	vector<double> angles = GetAngleData(adofai);
	vector<double> bpms;
	Physics phy;
	double height = 0;
	for (int i = 0; i < angles.size(); i++) {
		if (angles[i] == 999 || (i + 1 != angles.size() && angles[i + 1] == 999)) continue;
		height += sin(angles[i] / 360.0 * 2 * 3.141592653) * TILE_LENGTH;
		double bpm = phy.GetBPM(height, bpm_i);
		bpms.push_back(bpm);
	}

	cout << "> Setting ADOFAI File . . ." << endl;
	adofai["actions"].clear();
	adofai["actions"].append(ThreePlanet(1));
	for (int i = 1; i < bpms.size(); i++) {
		adofai["actions"].append(SetSpeed(i, "Bpm", bpms[i], 1));
	}

	cout << "> Saving ADOFAI File . . ." << endl;
	WriteJson(adofai, adofai_path);

	cout << endl << endl;
	cout << "< Completed!! - 완료되었습니다 >" << endl << endl;
	system("pause");
	return 0;
}