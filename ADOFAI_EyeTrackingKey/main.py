import cv2
import dlib
import imutils
import pyautogui
from scipy.spatial import distance as dist
from imutils import face_utils

cam = cv2.VideoCapture(0)

def calculate_EAR(eye):
	y1 = dist.euclidean(eye[1], eye[5])
	y2 = dist.euclidean(eye[2], eye[4])
	x1 = dist.euclidean(eye[0], eye[3])
	EAR = (y1+y2) / x1
	return EAR

(L_start, L_end) = face_utils.FACIAL_LANDMARKS_IDXS["left_eye"]
(R_start, R_end) = face_utils.FACIAL_LANDMARKS_IDXS['right_eye']

detector = dlib.get_frontal_face_detector()
landmark_predict = dlib.shape_predictor('shape_predictor_68_face_landmarks.dat')
prev = -1

while 1:
	_, frame = cam.read()
	frame = imutils.resize(frame, width=640)
	frame = cv2.flip(frame, 1)
	img_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
	faces = detector(img_gray)
	for face in faces:
		shape = landmark_predict(img_gray, face)
		shape = face_utils.shape_to_np(shape)
		lefteye = shape[L_start: L_end]
		righteye = shape[R_start:R_end]
		left_EAR = calculate_EAR(lefteye)
		right_EAR = calculate_EAR(righteye)
		avg = left_EAR + right_EAR
		if prev == -1:
			prev = avg
		if abs(prev - avg) > 0.2:
			if avg < prev:
				pyautogui.press('space')
			prev = avg

	#cv2.imshow("Video", frame)
	if cv2.waitKey(5) & 0xFF == ord('q'):
		break

cam.release()
cv2.destroyAllWindows()
