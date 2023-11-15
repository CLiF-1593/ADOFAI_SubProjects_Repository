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
constexpr double EndPoint = 2 * pi;
constexpr double Delta = 10;

constexpr bool UsePolar = false;
const double r(double theta) {
	//return theta; // Archimedean Spiral
	//return sin(theta); // Sinusoidal Spiral
	//return 1 + sin(theta); // Cardioid
	//return 1 + 2 * sin(theta); // Outer-Loop Limacon
	//return 2 + 1 * sin(theta); // Inner-Loop Limacon
	//return sin(theta * 4); // Rose (even)
	//return sin(theta * 5); // Rose (odd)
	//return sin(theta) + pow(sin(9 * theta / 2), 3); // Peacock
	//return sin(0.9 * theta) + pow(sin(9 * theta / 2), 5) + pow(cos(9 * theta / 2), 3); // Dandelion
	//return (exp(sin(theta)) - 2 * cos(4 * theta)) + pow(sin((2 * theta - pi) / 24), 5); // Butterfly
	//return pow(sin(1.2 * theta), 2) + pow(cos(6 * theta), 3); // Snowflake
	//return (sin(9.0 / 10.0 * theta)) * (3.0 - abs(5.0 * cos(9.0 / 10.0 * theta))); // Flower Curve
	return 3.5 - 1.5 * abs(cos(theta)) * sqrt(1.3 + abs(sin(theta))) + cos(2.0 * theta) - 3.0 * sin(theta) + 0.7 * cos(12.2 * theta); // Heart Curve
}

// Parametric Function Setting
const double x(double t) {
	if (UsePolar) return r(t) * cos(t);
	return -(721 * sin(t)) / 4 + 196 / 3 * sin(2 * t) - 86 / 3 * sin(3 * t) - 131 / 2 * sin(4 * t) + 477 / 14 * sin(5 * t) + 27 * sin(6 * t) - 29 / 2 * sin(7 * t) + 68 / 5 * sin(8 * t) + 1 / 10 * sin(9 * t) + 23 / 4 * sin(10 * t) - 19 / 2 * sin(12 * t) - 85 / 21 * sin(13 * t) + 2 / 3 * sin(14 * t) + 27 / 5 * sin(15 * t) + 7 / 4 * sin(16 * t) + 17 / 9 * sin(17 * t) - 4 * sin(18 * t) - 1 / 2 * sin(19 * t) + 1 / 6 * sin(20 * t) + 6 / 7 * sin(21 * t) - 1 / 8 * sin(22 * t) + 1 / 3 * sin(23 * t) + 3 / 2 * sin(24 * t) + 13 / 5 * sin(25 * t) + sin(26 * t) - 2 * sin(27 * t) + 3 / 5 * sin(28 * t) - 1 / 5 * sin(29 * t) + 1 / 5 * sin(30 * t) + (2337 * cos(t)) / 8 - 43 / 5 * cos(2 * t) + 322 / 5 * cos(3 * t) - 117 / 5 * cos(4 * t) - 26 / 5 * cos(5 * t) - 23 / 3 * cos(6 * t) + 143 / 4 * cos(7 * t) - 11 / 4 * cos(8 * t) - 31 / 3 * cos(9 * t) - 13 / 4 * cos(10 * t) - 9 / 2 * cos(11 * t) + 41 / 20 * cos(12 * t) + 8 * cos(13 * t) + 2 / 3 * cos(14 * t) + 6 * cos(15 * t) + 17 / 4 * cos(16 * t) - 3 / 2 * cos(17 * t) - 29 / 10 * cos(18 * t) + 11 / 6 * cos(19 * t) + 12 / 5 * cos(20 * t) + 3 / 2 * cos(21 * t) + 11 / 12 * cos(22 * t) - 4 / 5 * cos(23 * t) + cos(24 * t) + 17 / 8 * cos(25 * t) - 7 / 2 * cos(26 * t) - 5 / 6 * cos(27 * t) - 11 / 10 * cos(28 * t) + 1 / 2 * cos(29 * t) - 1 / 5 * cos(30 * t);
}

const double y(double t) {
	if (UsePolar) return r(t) * sin(t);
	return -(637 * sin(t)) / 2 - 188 / 5 * sin(2 * t) - 11 / 7 * sin(3 * t) - 12 / 5 * sin(4 * t) + 11 / 3 * sin(5 * t) - 37 / 4 * sin(6 * t) + 8 / 3 * sin(7 * t) + 65 / 6 * sin(8 * t) - 32 / 5 * sin(9 * t) - 41 / 4 * sin(10 * t) - 38 / 3 * sin(11 * t) - 47 / 8 * sin(12 * t) + 5 / 4 * sin(13 * t) - 41 / 7 * sin(14 * t) - 7 / 3 * sin(15 * t) - 13 / 7 * sin(16 * t) + 17 / 4 * sin(17 * t) - 9 / 4 * sin(18 * t) + 8 / 9 * sin(19 * t) + 3 / 5 * sin(20 * t) - 2 / 5 * sin(21 * t) + 4 / 3 * sin(22 * t) + 1 / 3 * sin(23 * t) + 3 / 5 * sin(24 * t) - 3 / 5 * sin(25 * t) + 6 / 5 * sin(26 * t) - 1 / 5 * sin(27 * t) + 10 / 9 * sin(28 * t) + 1 / 3 * sin(29 * t) - 3 / 4 * sin(30 * t) - (125 * cos(t)) / 2 - 521 / 9 * cos(2 * t) - 359 / 3 * cos(3 * t) + 47 / 3 * cos(4 * t) - 33 / 2 * cos(5 * t) - 5 / 4 * cos(6 * t) + 31 / 8 * cos(7 * t) + 9 / 10 * cos(8 * t) - 119 / 4 * cos(9 * t) - 17 / 2 * cos(10 * t) + 22 / 3 * cos(11 * t) + 15 / 4 * cos(12 * t) - 5 / 2 * cos(13 * t) + 19 / 6 * cos(14 * t) + 7 / 4 * cos(15 * t) + 31 / 4 * cos(16 * t) - cos(17 * t) + 11 / 10 * cos(18 * t) - 2 / 3 * cos(19 * t) + 13 / 3 * cos(20 * t) - 5 / 4 * cos(21 * t) + 2 / 3 * cos(22 * t) + 1 / 4 * cos(23 * t) + 5 / 6 * cos(24 * t) + 3 / 4 * cos(26 * t) - 1 / 2 * cos(27 * t) - 1 / 10 * cos(28 * t) - 1 / 3 * cos(29 * t) - 1 / 19 * cos(30 * t);
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