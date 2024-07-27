import cv2
from cvzone.HandTrackingModule import HandDetector
import pyautogui

cap = cv2.VideoCapture(0)

detector = HandDetector(maxHands=2, detectionCon=0.8)

keytable = ['f', 'g']
keypressed = [False, False]

while True:
    success, img = cap.read()
    # image size decrease
    img = cv2.resize(img, (320, 240))

    hands, img = detector.findHands(img)

    if hands:
        for i in range(len(hands)):
            hand = hands[i]
            lmList1 = hand["lmList"]
            bbox1 = hand["bbox"]
            centerPoint1 = hand['center']
            handType1 = hand["type"]
            cv2.line(img, lmList1[4][0:2], lmList1[8][0:2], (0, 255, 255), 2)

            pnt1 = lmList1[4][0:2]
            pnt2 = lmList1[8][0:2]
            distance = ((pnt1[0] - pnt2[0]) ** 2 + (pnt1[1] - pnt2[1]) ** 2) ** 0.5

            if handType1 == "Right":
                i = 0
            else:
                i = 1

            if distance < 20 and not keypressed[i]:
                pyautogui.keyDown(keytable[0])
                pyautogui.keyUp(keytable[1])
                keypressed[i] = True
            elif distance >= 20 and keypressed[i]:
                pyautogui.keyDown(keytable[1])
                pyautogui.keyUp(keytable[0])
                keypressed[i] = False

    cv2.imshow("Image", img)
    if cv2.waitKey(10) == ord("q"): # q 누를 시 웹켐 종료
        break