import cv2
from cvzone.HandTrackingModule import HandDetector

cap = cv2.VideoCapture(0)

detector = HandDetector(maxHands=2, detectionCon=0.8)

while True:
    success, img = cap.read()
    hands, img = detector.findHands(img)

    if hands:
        hand1 = hands[0]
        lmList1 = hand1["lmList"]
        bbox1 = hand1["bbox"]
        centerPoint1 = hand1['center']
        handType1 = hand1["type"]

        # draw line between thumb and index finger
        #print (lmList1[4], lmList1[8])
        cv2.line(img, lmList1[4][0:2], lmList1[8][0:2], (0, 255, 255), 2)

    cv2.imshow("Image", img)
    if cv2.waitKey(1) == ord("q"): # q 누를 시 웹켐 종료
        break