import json
import shutil
from copy import deepcopy
from multiprocessing import Process

import format

import cv2
import os

WIDTH = 720 // 2
HEIGHT = 480 // 2

FRAME = 3
THREAD = 4
DECORATION_NUM = 200

def main():
    #init()
    #frames = export_frames("video.mp4")
    #geometrize_thread(frames)
    frames = 101
    generate_adofai(frames)


def init():
    shutil.rmtree('frames', ignore_errors=True)
    shutil.rmtree('jsons', ignore_errors=True)
    os.mkdir("frames")
    os.mkdir("jsons")

def export_frames(video_path):
    cap = cv2.VideoCapture(video_path)
    i = 0
    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break
        if i % FRAME != 0:
            i += 1
            continue
        frame = cv2.resize(frame, dsize=(WIDTH, HEIGHT), interpolation=cv2.INTER_LINEAR)
        cv2.imwrite(os.path.join("frames", f"{i // FRAME}.png"), frame)
        i += 1
    cap.release()
    return i // FRAME

def geometrize_thread(frames):
    thread_pool = []
    for i in range(0, THREAD):
        begin = frames // THREAD * i
        end = frames // THREAD * (i + 1)
        if i == THREAD - 1: end = frames
        thread_pool.append(Process(target=geometrize, args=(begin, end)))
    for thread in thread_pool:
        thread.start()
    for thread in thread_pool:
        thread.join()

def geometrize(frame_begin_num, frame_end_num):
    for i in range(frame_begin_num, frame_end_num):
        os.system(f"geometrize_lib_example -i frames/{i}.png -o jsons/{i}_out.json -t rotated_rectangle -s " + str(DECORATION_NUM))

def generate_adofai(frame_num):
    with open('template.adofai', encoding='utf-8-sig') as json_file:
        adofai = json.load(json_file)

    for i in range(DECORATION_NUM) :
        format.deco["tag"] = f"deco{i}"
        format.deco["depth"] = -i - 10
        adofai["actions"].append(deepcopy(format.deco))

    tile_num = len(adofai["angleData"])
    offset = tile_num * 180.0 / frame_num
    for i in range(frame_num):
        with open(f'jsons/{i}_out.json') as json_file:
            data = json.load(json_file)
            cnt = 0
            for deco in data["shapes"]:
                adofai["actions"].append(deepcopy(format.set_deco_move(i * offset, deco, offset / 180.0, cnt)))
                adofai["actions"].append(deepcopy(format.set_deco_attribute(i * offset, deco, offset / 180.0, cnt)))
                cnt += 1
    #adofai["actions"].append(format.loop)

    with open('level.adofai', 'w') as json_file:
        json.dump(adofai, json_file)

if __name__ == "__main__":
    main()
