#pragma once
#include <string>
#include "json/json.h"
using namespace std;
enum Mode { INSIDE, OUTSIDE }; //내각 or 외각

class Editor {
private:
	Json::Value root;
public:
	void Open(string adofai_file_location); //파일 열기
	void ChangeFile(); //변형
	void Save(string adofai_file_location); //저장
};

