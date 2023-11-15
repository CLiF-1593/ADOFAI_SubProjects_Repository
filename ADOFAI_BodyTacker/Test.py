import copy
import json

def get_action(tile_num, bpm):
    json_action = {
        "floor": tile_num,
        "eventType": "SetSpeed",
        "speedType": "Bpm",
        "beatsPerMinute": bpm,
        "bpmMultiplier": 1,
        "angleOffset": 0
    }
    return json_action

with open('level_.adofai', 'r', encoding="utf-8-sig") as f:
    json_data = json.load(f)

for i in range(1, 4000):
    json_data["angleData"].append(0)
    json_data["actions"].append(get_action(i, 100 + 50 * i))

with open('edit.adofai', 'w', encoding="utf-8-sig") as f:
    json.dump(json_data, f, indent="\t", ensure_ascii=False)