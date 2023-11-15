#pragma once
#include "json/json.h"
using namespace std;

class Convertor {
private:
	vector<double> angle_data;
	vector<bool> twirl;

	Json::Value root;

	void SetAngle(double &angle);

public:
	bool Open(string adofai_file_location); //���� ����
	void ChangeFile(); //����
	void Save(string adofai_file_location); //����
};

