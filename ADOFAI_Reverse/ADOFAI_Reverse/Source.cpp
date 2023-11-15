#include <iostream>
#include <string>
#include "FileManager.h"
#include "BeatManager.h"
#define BPM_to_Time(x) (60000.0 / (x))

void SetAngle(double& angle) {
	while (angle < 0) {
		angle += 360;
	}
	while (angle >= 360) {
		angle -= 360;
	}
}

void AngleReverse(FileManager* file) {
	vector<double> data = file->GetAngleData();
	reverse(data.begin(), data.end());
	for (int i = 0; i < data.size(); i++) {
		if (data[i] != 999) {
			data[i] = 360 - data[i];
			SetAngle(data[i]);
		}
		else if (data[i] == 999) {
			auto t = data[i + 1];
			data[i + 1] = data[i];
			data[i] = t;
			i++;
		}
	}
	if (data[0] == 180) {
		for (int i = 0; i < data.size(); i++) {
			if (data[i] != 999 && (i == data.size() - 1 || data[i + 1] != 999)) {
				data[i] -= 180;
				SetAngle(data[i]);
			}
		}
	}
	file->SetAngleData(data);
}

void BPMReverse(FileManager* file) {
	int size = file->GetAngleData().size();
	auto data = file->GetBPMList();
	Effect e;
	e.tile_num = 0;
	e.value = file->GetBPM();
	data.insert(data.begin(), e);
	reverse(data.begin(), data.end());
	auto src = data;

	for (int i = 0; i < data.size(); i++) {
		if (i == 0) {
			data[i].tile_num = 0;
		}
		else {
			data[i].tile_num = size - src[i - 1].tile_num + 1;
		}
	}
	file->SetBPMList(data);
}

using namespace std;
int main() {
	srand((unsigned)time(NULL));

	//Title
	cout << "[ ADOFAI Reverse ]" << endl << endl;
	
	//Input File
	FileManager file;
	string loc;
	cout << "ADOFAI File Location : ";
	loc = "D:\\Game\\ADOFAI\\Content\\Reverse\\hyun_triple_cross\\main.adofai";
	//cin >> loc;
	if (!file.Read(loc)) {
		system("pause");
		return 1;
	}

	AngleReverse(&file);
	BPMReverse(&file);
	file.EffectReverse();
	file.Write(loc);

	return 0;
}