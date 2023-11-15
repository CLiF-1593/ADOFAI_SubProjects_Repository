#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <string>

bool FileManager::Read(string adofai_location) {
	Json::CharReaderBuilder reader;
	ifstream is(adofai_location, ifstream::binary);
	string errorMessage;
	auto bret = Json::parseFromStream(reader, is, &this->root, &errorMessage);

	if (bret == false) {
		cout << "[Log] FileManager" << endl;
		cout << "Error to parse JSON file" << endl << errorMessage << endl;
		return false;
	}

	return true;
}

bool FileManager::Write(string adofai_location) {
	FILE* f;
	fopen_s(&f, adofai_location.c_str(), "wb");
	fprintf_s(f, "%s", this->root.toStyledString().c_str());
	fclose(f);
	return false;
}

double FileManager::GetBPM() {
	return this->root["settings"]["bpm"].asDouble();
}

string FileManager::GetPathData() {
	return this->root["pathData"].asString();
}


vector<double> FileManager::GetAngleData() {
	Json::Value action = this->root["angleData"];
	vector<double> list;
	for (int i = 0; i < action.size(); i++) {
		list.push_back(action[i].asDouble());
	}
	if (list.empty()) {
		map<char, double> direction;
		direction.insert(pair<char, double>('R', 0));
		direction.insert(pair<char, double>('p', 15 * 1));
		direction.insert(pair<char, double>('J', 15 * 2));
		direction.insert(pair<char, double>('E', 15 * 3));
		direction.insert(pair<char, double>('T', 15 * 4));
		direction.insert(pair<char, double>('o', 15 * 5));
		direction.insert(pair<char, double>('U', 15 * 6));
		direction.insert(pair<char, double>('q', 15 * 7));
		direction.insert(pair<char, double>('G', 15 * 8));
		direction.insert(pair<char, double>('Q', 15 * 9));
		direction.insert(pair<char, double>('H', 15 * 10));
		direction.insert(pair<char, double>('W', 15 * 11));
		direction.insert(pair<char, double>('L', 15 * 12));
		direction.insert(pair<char, double>('x', 15 * 13));
		direction.insert(pair<char, double>('N', 15 * 14));
		direction.insert(pair<char, double>('Z', 15 * 15));
		direction.insert(pair<char, double>('F', 15 * 16));
		direction.insert(pair<char, double>('V', 15 * 17));
		direction.insert(pair<char, double>('D', 15 * 18));
		direction.insert(pair<char, double>('Y', 15 * 19));
		direction.insert(pair<char, double>('B', 15 * 20));
		direction.insert(pair<char, double>('C', 15 * 21));
		direction.insert(pair<char, double>('M', 15 * 22));
		direction.insert(pair<char, double>('A', 15 * 23));
		direction.insert(pair<char, double>('5', 72));
		direction.insert(pair<char, double>('6', -72));
		direction.insert(pair<char, double>('7', 360.0 / 7.0));
		direction.insert(pair<char, double>('8', -360.0 / 7.0));
		auto path = this->GetPathData();
		double special_angle = 0;
		for (int i = 0; i < path.size(); i++) {
			if (path[i] == '!') {
				list.push_back(999);
			}
			else if ('5' <= path[i] && path[i] <= '8') {
				special_angle += direction[path[i]];
				list.push_back(special_angle);
			}
			else {
				special_angle = direction[path[i]];
				list.push_back(special_angle);
			}
		}
	}
	return list;
}

void FileManager::SetAngleData(vector<double> data) {
	Json::Value *action = &this->root["angleData"];
	this->root.removeMember("pathData");
	action->clear();
	for (int i = 0; i < data.size(); i++) {
		action->append(data[i]);
	}
}

vector<Effect> FileManager::GetBPMList() {
	double bpm = this->GetBPM();
	Json::Value action = this->root["actions"];
	vector<Effect> list;
	for (int i = 0; i < action.size(); i++) {
		if (action[i]["eventType"].asString() == "SetSpeed") {
			if (action[i]["speedType"].asString() == "Multiplier") {
				bpm *= action[i]["bpmMultiplier"].asDouble();
			}
			else {
				bpm = action[i]["beatsPerMinute"].asDouble();
			}
			Effect element;
			element.tile_num = action[i]["floor"].asInt();
			element.value = bpm;
			list.push_back(element);
		}
	}
	return list;
}

void FileManager::SetBPMList(vector<Effect> bpm) {
	Json::Value *action = &this->root["actions"];
	for (int i = 0; i < action->size(); i++) {
		if ((*action)[i]["eventType"].asString() == "SetSpeed") {
			action->removeIndex(i, nullptr);
			i--;
		}
	}
	Json::Value v;
	v["eventType"] = "SetSpeed";
	v["speedType"] = "Bpm";
	for (int i = 0; i < bpm.size(); i++) {
		v["beatsPerMinute"] = bpm[i].value;
		v["floor"] = bpm[i].tile_num;
		if (bpm[i].tile_num == 0) {
			root["settings"]["bpm"] = bpm[i].value;
		}
		else {
			action->append(v);
		}
	}
}

queue<Effect> FileManager::GetTwirlList() {
	Json::Value action = this->root["actions"];
	queue<Effect> list;
	for (int i = 0; i < action.size(); i++) {
		if (action[i]["eventType"].asString() == "Twirl") {
			Effect element;
			element.tile_num = action[i]["floor"].asInt();
			element.value = 0;
			list.push(element);
		}
	}
	return list;
}

queue<Effect> FileManager::GetPlanetList() {
	Json::Value action = this->root["actions"];
	queue<Effect> list;
	for (int i = 0; i < action.size(); i++) {
		if (action[i]["eventType"].asString() == "MultiPlanet") {
			Effect element;
			element.tile_num = action[i]["floor"].asInt();
			element.value = action[i]["planets"].asInt();
			list.push(element);
		}
	}
	return list;
}

queue<Effect> FileManager::GetPauseList() {
	Json::Value action = this->root["actions"];
	queue<Effect> list;
	for (int i = 0; i < action.size(); i++) {
		if (action[i]["eventType"].asString() == "Pause") {
			Effect element;
			element.tile_num = action[i]["floor"].asInt();
			element.value = action[i]["duration"].asInt();;
			list.push(element);
		}
	}
	return list;
}

queue<Effect> FileManager::GetFreeRoamList() {
	Json::Value action = this->root["actions"];
	queue<Effect> list;
	for (int i = 0; i < action.size(); i++) {
		if (action[i]["eventType"].asString() == "FreeRoam") {
			Effect element;
			element.tile_num = action[i]["floor"].asInt();
			element.value = action[i]["duration"].asInt();;
			list.push(element);
		}
	}
	return list;
}

double FileManager::GetOffset() {
	double offset1 = this->root["settings"]["offset"].asDouble();
	double offset2 = (this->root["settings"]["countdownTicks"].asDouble()) * 60000 / this->GetBPM();
	return offset1 + offset2;
}

void FileManager::Log() {
	cout << "[Log] FileManager" << endl;
	cout << "root (type=Json)" << endl;
	cout << "data = ";
	cout << this->root;
	cout << endl;
}

void FileManager::EffectReverse() {
	Json::Value* action = &this->root["actions"];
	int size = this->GetAngleData().size();
	for (int i = 0; i < action->size(); i++) {
		if ((*action)[i]["eventType"].asString() != "SetSpeed") {
			(*action)[i]["floor"] = size - (*action)[i]["floor"].asInt() + 1;
		}
	}
}
