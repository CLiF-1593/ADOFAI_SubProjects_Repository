import cv2
import mediapipe as mp
import numpy as np

mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
mp_face_mesh = mp.solutions.face_mesh

drawing_spec = mp_drawing.DrawingSpec(thickness=1, circle_radius=1)
cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)

record_level = 0
dot_positions = []
eye_positions = []

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

        if record_level == 1:
            dot = []
            eye= []
            if results.multi_face_landmarks:
                for face_landmarks in results.multi_face_landmarks:
                    for i, landmark in enumerate(face_landmarks.landmark):
                        x = int(landmark.x * image.shape[1])
                        y = int(landmark.y * image.shape[0])
                        if i < 468:
                            dot.append((x, y))
                            cv2.circle(image, (x, y), 3, (0, 0, 255), -1)
                        else:
                            eye.append((x, y))
                            cv2.circle(image, (x, y), 3, (255, 255, 255), -1)
            dot_positions.append(dot)
            eye_positions.append(eye)

        if results.multi_face_landmarks:
            for face_landmarks in results.multi_face_landmarks:
                mp_drawing.draw_landmarks(
                    image=image,
                    landmark_list=face_landmarks,
                    connections=mp_face_mesh.FACEMESH_TESSELATION,
                    landmark_drawing_spec=None,
                    connection_drawing_spec=mp_drawing_styles
                    .get_default_face_mesh_tesselation_style())

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

import json

if record_level == 2:
    OFFSET = 15
    FPS = 10
    FRAME = 30

    unit = 60 // FPS + 1
    file_unit = unit * FRAME
    FILES = len(dot_positions) // file_unit

    if unit == 0: unit = 1

    for n in range(FILES):
        # open adofai as json (utf8-sig)
        with open("template.adofai", "r", encoding="utf-8-sig") as f:
            adofai = json.load(f)

        # add tile deco
        # select every two points from dot_positions[0]
        for i in range(len(dot_positions[0])):
            deco_txt_1 = '{ "floor": '
            floor = "0"
            deco_txt_2 = ', "eventType": "AddObject", "objectType": "Floor", "planetColorType": "DefaultRed", "planetColor": "ff0000", "planetTailColor": "ff0000", "trackType": "Normal", "trackAngle": 180, "trackColorType": "Single", "trackColor": "debb7b", "secondaryTrackColor": "ffffff", "trackColorAnimDuration": 2, "trackOpacity": 100, "trackStyle": "Standard", "trackIcon": "None", "trackIconAngle": 0, "trackIconFlipped": false, "trackRedSwirl": false, "trackGraySetSpeedIcon": false, "trackSetSpeedIconBpm": 100, "trackGlowEnabled": false, "trackGlowColor": "ffffff", "trackIconOutlines": false, "position": [0, 0], "relativeTo": "Tile", "pivotOffset": [0, 0], "rotation": 0, "lockRotation": false, "scale": [100, 100], "lockScale": false, "depth": -1, "syncFloorDepth": false, "parallax": [0, 0], "parallaxOffset": [0, 0], "tag": "'
            tag = str(i) + "_faceline"
            deco_txt_3 = '" }'
            deco = deco_txt_1 + floor + deco_txt_2 + tag + deco_txt_3
            adofai["decorations"].append(json.loads(deco))

        offset = 0
        for f in range(file_unit * n, file_unit * (n + 1), unit):
            for i in range(len(dot_positions[f])):
                FACTOR = 5
                x, y = dot_positions[f][i]
                x /= FACTOR
                y /= FACTOR

                action_txt_1 = '{ "floor": 1, "eventType": "MoveDecorations", "duration": '
                duration = OFFSET / 180
                action_txt_2 = ', "tag": "'
                tag = str(i) + "_faceline"
                action_txt_3 = '", "positionOffset": '
                position = [-x, -y]
                action_txt_4 = ', "rotationOffset": '
                rotation = 0
                action_txt_5 = ', "scale": '
                scale = [100, 100]
                action_txt_6 = ', "parallaxOffset": [null, null], "angleOffset": '
                angle_offset = offset
                action_txt_7 = ', "ease": "Linear", "eventTag": ""}'
                action = action_txt_1 + str(duration) + action_txt_2 + tag + action_txt_3 + str(
                    position) + action_txt_4 + str(rotation) + action_txt_5 + str(scale) + action_txt_6 + str(
                    angle_offset) + action_txt_7
                adofai["actions"].append(json.loads(action))
            offset += OFFSET

        with open("output_" + str(n) + ".adofai", "w", encoding="utf-8-sig") as f:
            json.dump(adofai, f, indent=4)

    print("녹화가 완료되었습니다.")
