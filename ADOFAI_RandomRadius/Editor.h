#pragma once
#include <string>
#include "json/json.h"
using namespace std;
enum Mode { INSIDE, OUTSIDE }; //���� or �ܰ�

class Editor {
private:
	Json::Value root;
public:
	void Open(string adofai_file_location); //���� ����
	void ChangeFile(); //����
	void Save(string adofai_file_location); //����
};

