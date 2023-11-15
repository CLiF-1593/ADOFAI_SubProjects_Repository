#pragma once
#include <string>
#include <queue>
#include "json/json.h"
using namespace std;

struct Effect {
	int tile_num;
	double value;
};

class FileManager {
private:
	Json::Value root;
	string GetPathData();

public:
	bool Read(string adofai_location);
	bool Write(string adofai_location);
	vector<double> GetAngleData();
	void SetAngleData(vector<double> data);
	double GetBPM(); 
	vector<Effect> GetBPMList(); //get bpm change data
	void SetBPMList(vector<Effect> bpm);
	queue<Effect> GetTwirlList(); //get twirl data
	queue<Effect> GetPlanetList(); //get planet data
	queue<Effect> GetPauseList(); //get pause data
	queue<Effect> GetFreeRoamList(); //get free roam data
	double GetOffset(); //not use
	void Log();

	void EffectReverse();
};

