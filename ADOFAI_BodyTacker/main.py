import copy
import json

import cv2
import mediapipe as mp
import time
import math


class poseDetector():
    def __init__(self, mode=False, smooth=True, detectionCon=0.5, trackCon=0.5):
        self.mode = mode
        self.smooth = smooth
        self.detectionCon = detectionCon
        self.trackCon = trackCon
        self.pTime = 0

        self.mpDraw = mp.solutions.drawing_utils
        self.mpPose = mp.solutions.pose
        self.pose = self.mpPose.Pose(static_image_mode=self.mode,
                                smooth_landmarks=self.smooth,
                                min_detection_confidence=self.detectionCon,
                                min_tracking_confidence=self.trackCon)

    def findPose(self, img, draw=True):
        imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        self.results = self.pose.process(imgRGB)

        if self.results.pose_landmarks:
            if draw:
                self.mpDraw.draw_landmarks(img, self.results.pose_landmarks, self.mpPose.POSE_CONNECTIONS)
        return img

    def getPosition(self, img):
        self.lmList = []
        if self.results.pose_landmarks:
            for id, lm in enumerate(self.results.pose_landmarks.landmark):
                h, w, c = img.shape
                cx, cy = int(lm.x * w), int(lm.y * h)
                self.lmList.append([id, cx, cy])
        return self.lmList

    def showFps(self, img):
        cTime = time.time()
        fps = 1 / (cTime - self.pTime)
        print(fps)
        self.pTime = cTime
        cv2.putText(img, str(int(fps)), (70, 80), cv2.FONT_HERSHEY_PLAIN, 3,
                    (255, 0, 0), 3)

    def findAngle(self, img, p1, p2, p3, draw=True):
        # Get the landmark
        x1, y1 = self.lmList[p1][1:]
        x2, y2 = self.lmList[p2][1:]
        x3, y3 = self.lmList[p3][1:]

        # Calculate the angle
        angle = math.degrees(math.atan2(y3 - y2, x3 - x2) - math.atan2(y1 - y2, x1 - x2))
        # some time this angle comes zero, so below conditon we added
        if angle < 0:
            angle += 360

        # Draw
        if draw:
            cv2.line(img, (x1, y1), (x2, y2), (255, 255, 255), 3)
            cv2.line(img, (x3, y3), (x2, y2), (255, 255, 255), 3)
            cv2.circle(img, (x1, y1), 10, (0, 0, 255), cv2.FILLED)
            cv2.circle(img, (x1, y1), 15, (0, 0, 255), 1)
            cv2.circle(img, (x2, y2), 10, (0, 0, 255), cv2.FILLED)
            cv2.circle(img, (x2, y2), 15, (0, 0, 255), 1)
            cv2.circle(img, (x3, y3), 10, (0, 0, 255), cv2.FILLED)
            cv2.circle(img, (x3, y3), 15, (0, 0, 255), 1)
            # cv2.putText(img, str(int(angle)), (x2 - 20, y2 + 50), cv2.FONT_HERSHEY_SIMPLEX,
            #             1, (0, 0, 255), 2)
        return angle

class Body:
    class Coord:
        x = 0
        y = 0
        def __init__(self, x, y):
            self.x = x
            self.y = y

    def __init__(self, lmList):
        self.head = self.Coord(lmList[0][1], lmList[0][2])
        self.neck = self.Coord((lmList[11][1] + lmList[12][1]) / 2, (lmList[11][2] + lmList[12][2]) / 2)
        self.core = self.Coord((lmList[23][1] + lmList[24][1]) / 2, (lmList[23][2] + lmList[24][2]) / 2)
        self.left_arm = self.Coord(lmList[13][1], lmList[13][2])
        self.right_arm = self.Coord(lmList[14][1], lmList[14][2])
        self.left_leg = self.Coord(lmList[25][1], lmList[25][2])
        self.right_leg = self.Coord(lmList[26][1], lmList[26][2])
        self.left_hand = self.Coord(lmList[15][1], lmList[15][2])
        self.right_hand = self.Coord(lmList[16][1], lmList[16][2])
        self.left_foot = self.Coord(lmList[27][1], lmList[27][2])
        self.right_foot = self.Coord(lmList[28][1], lmList[28][2])

    def distance(self, pos1, pos2):
        return math.sqrt((pos1.x - pos2.x) ** 2 + (pos1.y - pos2.y) ** 2)

    def draw(self, img):
        cv2.circle(img, (int(self.head.x), int(self.head.y)),int(self.distance(self.head, self.neck) * 0.7), (0, 0, 255), cv2.LINE_4)
        cv2.line(img, (int(self.head.x), int(self.head.y)), (int(self.neck.x), int(self.neck.y)), (255, 0, 0), 3)
        cv2.line(img, (int(self.neck.x), int(self.neck.y)), (int(self.core.x), int(self.core.y)), (255, 0, 0), 3)
        cv2.line(img, (int(self.neck.x), int(self.neck.y)), (int(self.left_arm.x), int(self.left_arm.y)), (255, 0, 0), 3)
        cv2.line(img, (int(self.neck.x), int(self.neck.y)), (int(self.right_arm.x), int(self.right_arm.y)), (255, 0, 0), 3)
        cv2.line(img, (int(self.core.x), int(self.core.y)), (int(self.left_leg.x), int(self.left_leg.y)), (255, 0, 0), 3)
        cv2.line(img, (int(self.core.x), int(self.core.y)), (int(self.right_leg.x), int(self.right_leg.y)), (255, 0, 0), 3)
        cv2.line(img, (int(self.left_arm.x), int(self.left_arm.y)), (int(self.left_hand.x), int(self.left_hand.y)), (255, 0, 0), 3)
        cv2.line(img, (int(self.right_arm.x), int(self.right_arm.y)), (int(self.right_hand.x), int(self.right_hand.y)), (255, 0, 0), 3)
        cv2.line(img, (int(self.left_leg.x), int(self.left_leg.y)), (int(self.left_foot.x), int(self.left_foot.y)), (255, 0, 0), 3)
        cv2.line(img, (int(self.right_leg.x), int(self.right_leg.y)), (int(self.right_foot.x), int(self.right_foot.y)), (255, 0, 0), 3)

    @staticmethod
    def get_action(tile_num = 0, x = 0.0, y = 0.0, scale = 100.0, degree = 0.0, offset = 0.0, duration = 1.0):
        json_action = {
            "floor" : 1,
            "eventType" : "MoveTrack",
            "startTile" : [tile_num, "Start"],
            "endTile" : [tile_num, "Start"],
            "gapLength" : 0,
            "duration" : duration,
            "positionOffset" : [x, y],
            "rotationOffset" : degree,
            "scale" : [scale, scale],
            "angleOffset" : offset,
            "ease" : "Linear",
            "maxVfxOnly" : "Disabled",
            "eventTag" : ""
        }
        return json_action

    def get_degree(self, y, x):
        return math.atan2(y, x) * 180 / math.pi

    tile_per_pixel = 0.01

    def tile_line(self, json, begin_tile_num, tile_cnt, begin, end, offset, duration):
        length = self.distance(begin, end) * self.tile_per_pixel
        scale = length / tile_cnt * 100.0
        degree = self.get_degree(-(end.y - begin.y), end.x - begin.x)
        x = begin.x * self.tile_per_pixel
        y = -begin.y * self.tile_per_pixel
        for i in range(begin_tile_num, tile_cnt + begin_tile_num):
            json["actions"].append(self.get_action(i, x, y, scale, degree, offset, duration))
            x += scale / 100 * math.cos(degree * math.pi / 180)
            y += scale / 100 * math.sin(degree * math.pi / 180)

    def set_json(self, json, offset, duration):
        core_tile_cnt = 2
        arm_tile_cnt = 2
        leg_tile_cnt = 2
        hand_tile_cnt = 2
        foot_tile_cnt = 2

        begin_tile_num = 2

        self.tile_line(json, begin_tile_num, hand_tile_cnt, self.left_arm, self.left_hand, offset, duration)
        begin_tile_num += hand_tile_cnt

        self.tile_line(json, begin_tile_num, hand_tile_cnt, self.right_arm, self.right_hand, offset, duration)
        begin_tile_num += hand_tile_cnt

        self.tile_line(json, begin_tile_num, arm_tile_cnt, self.neck, self.left_arm, offset, duration)
        begin_tile_num += arm_tile_cnt

        self.tile_line(json, begin_tile_num, arm_tile_cnt, self.neck, self.right_arm, offset, duration)
        begin_tile_num += arm_tile_cnt

        self.tile_line(json, begin_tile_num, foot_tile_cnt, self.left_leg, self.left_foot, offset, duration)
        begin_tile_num += foot_tile_cnt

        self.tile_line(json, begin_tile_num, foot_tile_cnt, self.right_leg, self.right_foot, offset, duration)
        begin_tile_num += foot_tile_cnt

        self.tile_line(json, begin_tile_num, leg_tile_cnt, self.core, self.left_leg, offset, duration)
        begin_tile_num += leg_tile_cnt

        self.tile_line(json, begin_tile_num, leg_tile_cnt, self.core, self.right_leg, offset, duration)
        begin_tile_num += leg_tile_cnt

        self.tile_line(json, begin_tile_num, core_tile_cnt, self.neck, self.core, offset, duration)
        begin_tile_num += core_tile_cnt

        json["actions"].append(self.get_action(begin_tile_num, self.head.x * self.tile_per_pixel, -self.head.y * self.tile_per_pixel + 0.5, 1.5 * self.distance(self.core, self.neck), 0, offset, duration))

        return json

#  "floor": 1, "eventType": "MoveTrack", "startTile": [0, "ThisTile"], "endTile": [0, "ThisTile"], "gapLength": 0, "duration": 1, "positionOffset": [null, null], "rotationOffset": 0, "scale": [100, 100], "angleOffset": 0, "ease": "Linear", "maxVfxOnly": "Disabled", "eventTag": "", 

def main():
    detector = poseDetector()
    cap = cv2.VideoCapture("video_dance.mp4")

    w = round(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    h = round(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fps = cap.get(cv2.CAP_PROP_FPS)
    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    out = cv2.VideoWriter('tracking.mp4', fourcc, fps, (w, h))

    body_list = []

    while True:
        success, img = cap.read()
        if not success: break
        img = detector.findPose(img, draw=False)
        lmList = detector.getPosition(img)

        if lmList:
            body = Body(lmList)
            body.draw(img)
            body_list.append(body)
        elif body_list:
            body_list.append(body_list[-1])

        #detector.showFps(img)
        #cv2.imshow("Image", img)
        #cv2.waitKey(1)

        out.write(img)

    out.release()
    cap.release()

    with open('level.adofai', 'r', encoding="utf-8-sig") as f:
        json_data = json.load(f)

    for i in range(0, len(body_list), 2):
        body_list[i].set_json(json_data, 180 / len(body_list) * i, 1 / len(body_list))

    with open('dance.adofai', 'w', encoding="utf-8-sig") as f:
        json.dump(json_data, f, indent="\t", ensure_ascii=False)

if __name__ == "__main__":
    main()