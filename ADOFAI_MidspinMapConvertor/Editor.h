#pragma once
#include <string>
#include "json/json.h"
using namespace std;
enum Mode { INSIDE, OUTSIDE }; //���� or �ܰ�
struct Range {
	int begin;
	int end;
};

class Editor {
private:
	vector<double> angle_data;
	Range range;
	double bpm;

	Json::Value root;

	void ClearEffects();

	void SetAngle(double *angle);

public:
	void Open(string adofai_file_location); //���� ����
 	void SetRange(int start_tile, int end_tile); //���� ���� ����
	void SetBPM(double bpm); //BPM ����
	void ChangeFile(); //����
	void Save(string adofai_file_location); //����
};

