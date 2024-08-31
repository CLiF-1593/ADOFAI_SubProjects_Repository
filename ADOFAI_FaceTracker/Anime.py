import math

import cv2
import mediapipe as mp
import numpy as np
from scipy.spatial import ConvexHull
import matplotlib.pyplot as plt

mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
mp_face_mesh = mp.solutions.face_mesh

drawing_spec = mp_drawing.DrawingSpec(thickness=1, circle_radius=1)
cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)

record_level = 0

def distance(p1, p2):
    return (p1[0] - p2[0]) ** 2 + (p1[1] - p2[1]) ** 2

X_FACTOR = 1.05
Y_FACTOR = 0.95
CENTER = [0,0]

def change_dots(point, x_factor = X_FACTOR, y_factor = Y_FACTOR):
    center = CENTER
    dx = point[0] - center[0]
    dy = point[1] - center[1]
    return int(center[0] + dx * x_factor), int(center[1] + dy * y_factor)

record = []
eye_record = []

with mp_face_mesh.FaceMesh(
        max_num_faces=1,
        refine_landmarks=True,
        min_detection_confidence=0.5,
        min_tracking_confidence=0.5) as face_mesh:
    while cap.isOpened():
        success, image = cap.read()
        if not success:
            print("웹캠을 찾을 수 없습니다.")
            break

        image.flags.writeable = False
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        results = face_mesh.process(image)

        image.flags.writeable = True
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

        # create black image
        image = np.zeros(image.shape, dtype=np.uint8)

        CENTER = results.multi_face_landmarks[0].landmark[4].x, results.multi_face_landmarks[0].landmark[4].y

        dot = []
        if results.multi_face_landmarks:
            for face_landmarks in results.multi_face_landmarks:
                for i, landmark in enumerate(face_landmarks.landmark):
                    x = int(landmark.x * image.shape[1])
                    y = int(landmark.y * image.shape[0])
                    x, y = change_dots((x, y))
                    dot.append((x, y))

        try :
            hull_vertices = ConvexHull(dot).vertices
        except:
            hull_vertices = []

        face_size = 0
        min_y, max_y = 0, 0

        lines = []

        for i in range(len(hull_vertices)):
            try:
                x1, y1 = dot[hull_vertices[i]]
                x2, y2 = dot[hull_vertices[(i + 1) % len(hull_vertices)]]
                cv2.line(image, (x1, y1), (x2, y2), (0, 0, 255), 2)
                lines.append((x1, y1, x2, y2))
                min_y = min(min_y, y1, y2)
                max_y = max(max_y, y1, y2)
            except:
                pass
        face_size = max_y - min_y

        # NOSE
        try:
            x1, y1 = dot[1]
            x2, y2 = dot[4]

            y1 += face_size // 25
            y2 += face_size // 25

            cv2.line(image, (x1, y1), (x2, y2), (0, 0, 255), 2)
            lines.append((x1, y1, x2, y2))
        except:
            pass

        # LIPS
        mouth_center = (dot[13][0] + dot[14][0]) // 2, (dot[13][1] + dot[14][1]) // 2
        def change_mouth(point, x_factor, y_factor):
            center = mouth_center
            dx = point[0] - center[0]
            dy = point[1] - center[1]

            return int(center[0] + dx * x_factor), int(center[1] + dy * y_factor)

        FACEMESH_MOUTH = frozenset([(78, 95), (95, 88), (88, 178), (178, 87), (87, 14),
                                    (14, 317), (317, 402), (402, 318), (318, 324),
                                    (324, 308), (78, 191), (191, 80), (80, 81), (81, 82),
                                    (82, 13), (13, 312), (312, 311), (311, 310),
                                    (310, 415), (415, 308)])

        for idx in FACEMESH_MOUTH:
            try:
                x1, y1 = dot[idx[0]]
                x2, y2 = dot[idx[1]]
                x1, y1 = change_mouth((x1, y1), 0.7, 0.7)
                x2, y2 = change_mouth((x2, y2), 0.7, 0.7)

                y1 += face_size // 35
                y2 += face_size // 35

                cv2.line(image, (x1, y1), (x2, y2), (0, 0, 255), 2)
                lines.append((x1, y1, x2, y2))
            except:
                pass

        # EYES
        eye_center_1 = dot[468]
        eye_center_2 = dot[473]
        def extend(point, x_factor, y_factor):
            center = eye_center_1 if distance(point, eye_center_1) < distance(point, eye_center_2) else eye_center_2
            dx = point[0] - center[0]
            dy = point[1] - center[1]
            return int(center[0] + dx * x_factor), int(center[1] + dy * y_factor)


        FACEMESH_EYE_LEFT = frozenset([(263, 249), (249, 390), (390, 373), (373, 374),
                                (374, 380), (380, 381), (381, 384),
                                (263, 466), (466, 388), (388, 387), (387, 386),
                                (386, 385), (385, 384)])
        FACEMESH_EYE_RIGHT = frozenset([(33, 7), (7, 163), (163, 144), (144, 145),
                                (145, 153), (153, 154), (154, 157),
                                (33, 246), (246, 161), (161, 160), (160, 159),
                                (159, 158), (158, 157)])

        eye_left_dy = 0
        eye_right_dy = 0

        for idx in FACEMESH_EYE_LEFT | FACEMESH_EYE_RIGHT:
            try:
                x1, y1 = dot[idx[0]]
                x2, y2 = dot[idx[1]]

                x1, y1 = extend((x1, y1), 1.5, 2.8)
                x2, y2 = extend((x2, y2), 1.5, 2.8)

                y1 += face_size // 12
                y2 += face_size // 12

                cv2.line(image, (x1, y1), (x2, y2), (255, 255, 255), 2)
                lines.append((x1, y1, x2, y2))

                if idx in FACEMESH_EYE_LEFT:
                    eye_left_dy = max(eye_left_dy, abs(y1 - y2))
                else:
                    eye_right_dy = max(eye_right_dy, abs(y1 - y2))

            except:
                pass

        def eye(p1, p2):
            x1, y1 = p1
            x2, y2 = p2

            x1, y1 = extend((x1, y1), 1.4, 2.2)
            x2, y2 = extend((x2, y2), 1.4, 2.2)

            y1 += face_size // 12
            y2 += face_size // 12

            return max(abs(x1 - x2), abs(y1 - y2))

        eye_dx_1 = eye(dot[471], dot[469])
        eye_dy_1 = eye_right_dy
        eye_dx_2 = eye(dot[474], dot[476])
        eye_dy_2 = eye_left_dy
        eye_center_1 = (eye_center_1[0], eye_center_1[1] + face_size // 12)
        eye_center_2 = (eye_center_2[0], eye_center_2[1] + face_size // 12)

        if record_level == 1:
            record.append(lines)
            eye_record.append((eye_center_1, eye_center_2, eye_dx_1, eye_dy_1, eye_dx_2, eye_dy_2))

        cv2.imshow('MediaPipe Face Mesh(Puleugo)', cv2.flip(image, 1))
        if cv2.waitKey(1) & 0xFF == 27:
            record_level += 1
            if record_level == 1:
                print("녹화를 시작합니다.")
            elif record_level == 2:
                print("녹화를 종료합니다.")
                break

        if record_level == 2:
            break
cap.release()

line_size = 0
for i in range(len(record)):
    line_size = max(line_size, len(record[i]))

for i in range(len(record)):
    record[i].reverse()
    while len(record[i]) < line_size:
        record[i].append(record[i][-1])

import json

if record_level == 2:
    OFFSET = 30
    FPS = 60
    FRAME = 100

    unit = math.ceil(60 / FPS)
    file_unit = unit * FRAME
    FILES = len(record) // file_unit + 1

    if unit == 0: unit = 1

    for n in range(FILES):
        # open adofai as json (utf8-sig)
        with open("template.adofai", "r", encoding="utf-8-sig") as f:
            adofai = json.load(f)

        # add tile deco
        # select every two points from dot_positions[0]
        prev_angle = [0 for _ in range(len(record[0]))]
        for i in range(len(record[0])):
            deco_txt_1 = '{ "floor": '
            floor = "0"
            deco_txt_2 = ', "eventType": "AddObject", "objectType": "Floor", "planetColorType": "DefaultRed", "planetColor": "ff0000", "planetTailColor": "ff0000", "trackType": "Normal", "trackAngle": 180, "trackColorType": "Single", "trackColor": "debb7b", "secondaryTrackColor": "ffffff", "trackColorAnimDuration": 2, "trackOpacity": 100, "trackStyle": "Standard", "trackIcon": "None", "trackIconAngle": 0, "trackIconFlipped": false, "trackRedSwirl": false, "trackGraySetSpeedIcon": false, "trackSetSpeedIconBpm": 100, "trackGlowEnabled": false, "trackGlowColor": "ffffff", "trackIconOutlines": false, "position": [0, 0], "relativeTo": "Tile", "pivotOffset": [0, 0], "rotation": 0, "lockRotation": false, "scale": [100, 100], "lockScale": false, "depth": -1, "syncFloorDepth": false, "parallax": [0, 0], "parallaxOffset": [0, 0], "tag": "'
            tag = str(i) + "_faceline"
            deco_txt_3 = '" }'
            deco = deco_txt_1 + floor + deco_txt_2 + tag + deco_txt_3
            adofai["decorations"].append(json.loads(deco))

        deco = '{ "eventType": "AddObject", "objectType": "Planet", "planetColorType": "DefaultRed", "planetColor": "ff0000", "planetTailColor": "ff0000", "trackType": "Normal", "trackAngle": 180, "trackColorType": "Single", "trackColor": "debb7b", "secondaryTrackColor": "ffffff", "trackColorAnimDuration": 2, "trackOpacity": 100, "trackStyle": "Standard", "trackIcon": "None", "trackIconAngle": 0, "trackIconFlipped": false, "trackRedSwirl": false, "trackGraySetSpeedIcon": false, "trackSetSpeedIconBpm": 100, "trackGlowEnabled": false, "trackGlowColor": "ffffff", "trackIconOutlines": false, "position": [0, 0], "relativeTo": "Global", "pivotOffset": [0, 0], "rotation": 0, "lockRotation": false, "scale": [100, 100], "lockScale": false, "depth": -1, "syncFloorDepth": false, "parallax": [0, 0], "parallaxOffset": [0, 0], "tag": "1_eye" }'
        adofai["decorations"].append(json.loads(deco))
        deco = '{ "eventType": "AddObject", "objectType": "Planet", "planetColorType": "DefaultBlue", "planetColor": "ff0000", "planetTailColor": "ff0000", "trackType": "Normal", "trackAngle": 180, "trackColorType": "Single", "trackColor": "debb7b", "secondaryTrackColor": "ffffff", "trackColorAnimDuration": 2, "trackOpacity": 100, "trackStyle": "Standard", "trackIcon": "None", "trackIconAngle": 0, "trackIconFlipped": false, "trackRedSwirl": false, "trackGraySetSpeedIcon": false, "trackSetSpeedIconBpm": 100, "trackGlowEnabled": false, "trackGlowColor": "ffffff", "trackIconOutlines": false, "position": [0, 0], "relativeTo": "Global", "pivotOffset": [0, 0], "rotation": 0, "lockRotation": false, "scale": [100, 100], "lockScale": false, "depth": -1, "syncFloorDepth": false, "parallax": [0, 0], "parallaxOffset": [0, 0], "tag": "2_eye" }'
        adofai["decorations"].append(json.loads(deco))

        offset = 0
        for f in range(file_unit * n, min(file_unit * (n + 1), len(record)), unit):
            FACTOR = 5
            for i in range(len(record[f])):
                x1, y1, x2, y2 = record[f][i]
                x1 /= -FACTOR
                y1 /= -FACTOR
                x2 /= -FACTOR
                y2 /= -FACTOR

                action_txt_1 = '{ "floor": 1, "eventType": "MoveDecorations", "duration": '
                duration = OFFSET / 180
                if i >= 47: duration = 0
                action_txt_2 = ', "tag": "'
                tag = str(i) + "_faceline"
                action_txt_3 = '", "positionOffset": '
                position = [(x1 + x2) / 2, (y1 + y2) / 2]
                action_txt_4 = ', "rotationOffset": '
                rotation = math.atan2(y2 - y1, x2 - x1) * 180 / math.pi

                while rotation - prev_angle[i] > 90:
                    rotation -= 180
                while rotation - prev_angle[i] < -90:
                    rotation += 180

                prev_angle[i] = rotation

                action_txt_5 = ', "scale": '
                dist = distance((x1, y1), (x2, y2)) ** 0.5
                scale = [dist * 100, 100]
                action_txt_6 = ', "parallaxOffset": [null, null], "angleOffset": '
                angle_offset = offset
                action_txt_7 = ', "ease": "Linear", "eventTag": ""}'
                action = action_txt_1 + str(duration) + action_txt_2 + tag + action_txt_3 + str(
                    position) + action_txt_4 + str(rotation) + action_txt_5 + str(scale) + action_txt_6 + str(
                    angle_offset) + action_txt_7
                adofai["actions"].append(json.loads(action))

            eye_center_1, eye_center_2, eye_dx_1, eye_dy_1, eye_dx_2, eye_dy_2 = eye_record[f]
            eye_center_1 = list(eye_center_1)
            eye_center_2 = list(eye_center_2)
            eye_center_1[0] /= -FACTOR
            eye_center_2[0] /= -FACTOR
            eye_center_1[1] /= -FACTOR
            eye_center_2[1] /= -FACTOR
            eye_dx_1 /= FACTOR
            eye_dy_1 /= FACTOR
            eye_dx_2 /= FACTOR
            eye_dy_2 /= FACTOR

            action_txt_1 = '{ "floor": 1, "eventType": "MoveDecorations", "duration": '
            action_txt_2 = ', "tag": "'
            action_txt_3 = '", "positionOffset": '
            action_txt_4 = ', "rotationOffset": '
            action_txt_5 = ', "scale": '
            action_txt_6 = ', "parallaxOffset": [null, null], "angleOffset": '
            action_txt_7 = ', "ease": "Linear", "eventTag": ""}'

            duration = OFFSET / 180
            tag = "1_eye"
            position = eye_center_1
            rotation = 0
            scale = [200 * eye_dx_1, 300 * eye_dy_1]
            if 300 * eye_dy_1 < 300 :
                scale = [0, 0]
            angle_offset = offset
            action = action_txt_1 + str(duration) + action_txt_2 + tag + action_txt_3 + str(
                position) + action_txt_4 + str(rotation) + action_txt_5 + str(scale) + action_txt_6 + str(
                angle_offset) + action_txt_7
            adofai["actions"].append(json.loads(action))

            tag = "2_eye"
            position = eye_center_2
            rotation = 0
            scale = [200 * eye_dx_2, 300 * eye_dy_2]
            if 300 * eye_dy_2 < 300 :
                scale = [0, 0]
            angle_offset = offset
            action = action_txt_1 + str(duration) + action_txt_2 + tag + action_txt_3 + str(
                position) + action_txt_4 + str(rotation) + action_txt_5 + str(scale) + action_txt_6 + str(
                angle_offset) + action_txt_7
            adofai["actions"].append(json.loads(action))

            offset += OFFSET
            if f == file_unit * n:
                offset += 180

        with open("output_" + str(n) + ".adofai", "w", encoding="utf-8-sig") as f:
            json.dump(adofai, f, indent=4)

    print("녹화가 완료되었습니다.")
