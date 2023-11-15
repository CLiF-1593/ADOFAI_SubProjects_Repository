#include <iostream>
#include <string>
#include <Windows.h>
#include <ctime>
#include <vector>
#include "File.h"
#include "EventJson.h"
#include "AngleData.h"
#include "Physics.h"
#include "EasingFunctionSet.h"
using namespace std;

#define pi 3.14159265

// derived function
double function(double x) {
	//return 5 * x * x * x * x - 20 * x * x * x + 30 * x * x - 20 * x + 5;
	if (x < 0.5) {
		return 80 * x * x * x * x;
	}
	return 80 * x * x * x * x - 320 * x * x * x + 480 * x * x - 320 * x + 80;
}

int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	cout << "[ ADOFAI Formula To ADOFAI Map ]" << endl << endl;
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

	vector<double> angles;

	double x = 0;
	double range = 1.0;
	double x_delta = 0.02;

	auto Function = function;

	while (x <= range) {
		double slope = Function(x);
		double angle = atan(slope);
		x += cos(angle) * x_delta;
		angles.push_back(angle * 360 / 2.0 / 3.14159265);
	}

	cout << "> Setting ADOFAI File . . ." << endl;
	
	adofai["angleData"].clear();

	for (int i = 0; i < angles.size(); i++) {
		adofai["angleData"].append(angles[i]);
	}
	
	adofai["actions"].clear();

	for (int i = 2; i < angles.size(); i++) {
		double delta = (angles[i] + 180) - angles[i - 1];
		if (delta < 0.0) delta += 360.0;
		double bpm = 100;
		bpm *= 180.0 / delta;
		adofai["actions"].append(SetSpeed(i, "Bpm", bpm, 1));
	}

	cout << "> Saving ADOFAI File . . ." << endl;
	WriteJson(adofai, adofai_path);

	cout << endl << endl;
	cout << "< Completed!! - 완료되었습니다 >" << endl << endl;
	system("pause");
	return 0;
}