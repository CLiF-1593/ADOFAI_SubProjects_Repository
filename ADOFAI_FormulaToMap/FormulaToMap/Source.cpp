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

constexpr bool ConstBPM = true;
constexpr int TargetBPM = 800;

// Polar Function Setting
constexpr double EndPoint = 4 * pi;
constexpr double Delta = 0.1;

constexpr bool UsePolar = true;
const double r(double theta) {
	//return theta; // Archimedean Spiral
	return sin(theta); // Sinusoidal Spiral
	return 1 + sin(theta); // Cardioid
	return 1 + 2 * sin(theta); // Outer-Loop Limacon
	return 2 + 1 * sin(theta); // Inner-Loop Limacon
	return sin(theta * 4); // Rose (even)
	return sin(theta * 5); // Rose (odd)
	//return sin(theta) + pow(sin(9 * theta / 2), 3); // Peacock
	//return sin(0.9 * theta) + pow(sin(9 * theta / 2), 5) + pow(cos(9 * theta / 2), 3); // Dandelion
	//return (exp(sin(theta)) - 2 * cos(4 * theta)) + pow(sin((2 * theta - pi) / 24), 5); // Butterfly
	//return pow(sin(1.2 * theta), 2) + pow(cos(6 * theta), 3); // Snowflake
	//return (sin(9.0 / 10.0 * theta)) * (3.0 - abs(5.0 * cos(9.0 / 10.0 * theta))); // Flower Curve
}

// Parametric Function Setting
const double x(double t) {
	if (UsePolar) return r(t) * cos(t);
	return t;
}
const double y(double t) {
	if (UsePolar) return r(t) * sin(t);
	return t * t;
}

double SetAngle(double x) {
	double d = x * 1000.0;
	return (double)(round(d)) / 1000.0;
}

double GetT(double t) {
	double x0 = x(t);
	double y0 = y(t);
	double dt = 1;
	for (int i = 0; i < 8; i++) {
		double x1 = x0, y1 = y0;
		int j = 0;
		while ((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1) < Delta * Delta) {
			j++;
			x1 = x(t + dt * j);
			y1 = y(t + dt * j);
		}
		t += dt * (j - 1);
		dt *= 0.1;
	}
	return t;
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

	for (double i = 0; i < EndPoint;) {
		double x0 = x(i);
		double y0 = y(i);
		i = GetT(i);
		double x1 = x(i);
		double y1 = y(i);
		double angle = atan2(y1 - y0, x1 - x0);

		angles.push_back(SetAngle(angle * 360 / 2.0 / pi));
	}

	cout << "> Setting ADOFAI File . . ." << endl;
	
	adofai["angleData"].clear();

	for (int i = 0; i < angles.size(); i++) {
		adofai["angleData"].append(angles[i]);
	}
	
	adofai["actions"].clear();

	if (ConstBPM) {
		for (int i = 1; i < angles.size(); i++) {
			double delta = angles[i] - (angles[i - 1] + 180);
			while (delta < 0.0) delta += 360.0;
			while (delta >= 360.0) delta -= 360.0;
			double bpm = TargetBPM / delta * 180.0;
			adofai["actions"].append(SetSpeed(i, "Bpm", bpm, 1));
		}
	}
	else {
		for (int i = 2; i < angles.size(); i++) {
			double delta = angles[i] - (angles[i - 1] + 180);
			while (delta < 0.0) delta += 360.0;
			while (delta >= 360.0) delta -= 360.0;

			double prev = (angles[i - 1] + 180) * 2 * pi / 360;
			double next = (angles[i]) * 2 * pi / 360;
			double move = abs(cos(next) - cos(prev));

			double bpm = TargetBPM / move;
			bpm *= 180.0 / delta;
			adofai["actions"].append(SetSpeed(i, "Bpm", bpm, 1));
		}
	}

	adofai["settings"]["position"][0] = -x(0) / Delta;
	adofai["settings"]["position"][1] = y(0) / Delta;

	cout << "> Saving ADOFAI File . . ." << endl;
	WriteJson(adofai, adofai_path);

	cout << endl << endl;
	cout << "< Completed!! - 완료되었습니다 >" << endl << endl;
	system("pause");
	return 0;
}