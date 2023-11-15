#include "EventJson.h"

Json::Value AddDecoration(int floor, string deco_tag) {
    Json::Value ret;
    ret["floor"] = floor;
    ret["eventType"] = "AddDecoration";
    ret["decorationImage"] = "";
    ret["position"].append(0);
    ret["position"].append(0);
    ret["relativeTo"] = "Tile";
    ret["pivotOffset"].append(0);
    ret["pivotOffset"].append(0);
    ret["rotation"] = 0;
    ret["scale"].append(100);
    ret["scale"].append(100);
    ret["tile"].append(1);
    ret["tile"].append(1);
    ret["color"] = "ffffff";
    ret["opacity"] = 100;
    ret["depth"] = -1;
    ret["parallax"].append(0);
    ret["parallax"].append(0);
    ret["tag"] = deco_tag;
    ret["imageSmoothing"] = "Enabled";
	ret["blendMode"] = "None";
	ret["maskingType"] = "None";
	ret["useMaskingDepth"] = "Disabled";
	ret["maskingFrontDepth"] = -1;
	ret["maskingBackDepth"] = -1;
    ret["failHitbox"] = "Disabled";
    ret["failHitboxType"] = "Box";
    ret["failHitboxScale"].append(100);
    ret["failHitboxScale"].append(100);
    ret["failHitboxOffset"].append(0);
    ret["failHitboxOffset"].append(0);
    ret["failHitboxRotation"] = 0;
    ret["components"] = "";
    return ret;
}

Json::Value MoveDecorations(int floor, double angle_offset, string file_name, string deco_tag, string event_tag) {
    Json::Value ret;
    ret["floor"] = floor;
    ret["eventType"] = "MoveDecorations";
    ret["duration"] = 0;
    ret["tag"] = deco_tag;
    ret["decorationImage"] = file_name;
    ret["angleOffset"] = angle_offset;
    ret["ease"] = "Linear";
    ret["eventTag"] = event_tag;
    return ret;
}

Json::Value RepeatEvents(int floor, string event_tag, double interval, int repetitions) {
    Json::Value ret;
    ret["floor"] = floor;
    ret["eventType"] = "RepeatEvents";
    ret["repetitions"] = repetitions;
    ret["interval"] = interval;
    ret["tag"] = event_tag;
    return ret;
}

Json::Value SetSpeed(int floor, string speed_type, double bpm, double multiplier) {
	Json::Value ret;
	ret["floor"] = floor;
	ret["eventType"] = "SetSpeed";
	ret["speedType"] = speed_type;
	ret["beatsPerMinute"] = bpm;
	ret["bpmMultiplier"] = multiplier;
	ret["angleOffset"] = 0;
	return ret;
}

Json::Value ThreePlanet(int floor) {
	Json::Value ret;
	ret["floor"] = floor;
	ret["eventType"] = "MultiPlanet";
	ret["planets"] = "ThreePlanets";
	return ret;
}
