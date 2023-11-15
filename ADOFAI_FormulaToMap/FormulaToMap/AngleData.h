#include <iostream>
#include <vector>
#include "json/json.h"
using namespace std;

vector<double> GetAngleData(Json::Value adofai) {
	Json::Value action = adofai["angleData"];
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
		auto path = adofai["pathData"].asString();
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