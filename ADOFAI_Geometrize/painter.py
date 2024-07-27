import json
import shutil
from copy import deepcopy
from multiprocessing import Process

import format

import cv2
import os

WIDTH = 1200
HEIGHT = 675

DECORATION_NUM = 500

def main():
    #geometrize("Image.png")
    generate_adofai()

def geometrize(image):
    os.system(f"geometrize_lib -i {image} -o out.json -t rotated_rectangle -s " + str(DECORATION_NUM))

def generate_adofai():
    with open('level.adofai', encoding='utf-8-sig') as json_file:
        adofai = json.load(json_file)

    #for i in range(DECORATION_NUM) :
    #    adofai["angleData"].append(0)
    #    adofai["actions"].append(deepcopy(format.set_tile_move_to_end(i, 0, 0)))
    #    format.deco["tag"] = f"deco{i}"
    #    format.deco["depth"] = -i - 10
    #    adofai["actions"].append(deepcopy(format.deco))

    with open('out.json') as json_file:
        data = json.load(json_file)
        cnt = 0
        for deco in data["shapes"]:
            adofai["actions"].append(deepcopy(format.set_deco_move(0, deco, 1, cnt, -8, 13, cnt + 1)))
            adofai["actions"].append(deepcopy(format.set_deco_attribute(0, deco, 0, cnt, cnt + 1)))
            cnt += 1

    with open('level.adofai', 'w') as json_file:
        json.dump(adofai, json_file)

if __name__ == "__main__":
    main()
