#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
using namespace std;

#define ADOFAI_CAPTION L"A Dance of Fire and Ice"

int main() {
	cout << "[ ADOFAI Screen Resizer ]" << endl << endl;
	cout << "얼불춤 화면 비율을 자유롭게 조정해줍니다." << endl;
	cout << "Developer : CLiF" << endl;
	cout << "Version : v 1.1" << endl << endl;

	int ww = 0;
	int hh = 0;
	cout << "Width : ";
	cin >> ww;
	cout << "Height : ";
	cin >> hh;

	HWND hwnd = FindWindow(NULL, ADOFAI_CAPTION);
	if (!hwnd) {
		cout << "[LOG] ADOFAI Window Finding Error - Cannot found" << endl;
		system("pause");
		return 1;
	}
	RECT win;
	GetWindowRect(hwnd, &win);

	/*while (true) {
		SetWindowPos(hwnd, HWND_TOPMOST, (1920 - 1920) / 2, (1080 - 1080) / 2, 1920, 1080, SWP_SHOWWINDOW);
		SetWindowPos(hwnd, HWND_TOPMOST, (1920 - 1600) / 2, (1080 - 900) / 2, 1600, 900, SWP_SHOWWINDOW);
		SetWindowPos(hwnd, HWND_TOPMOST, (1920 - 1080) / 2, (1080 - 720) / 2, 1080, 720, SWP_SHOWWINDOW);
	}*/
	
	for (int i = 0; i <= 100; i++) {
		int w = (ww * i + (win.right - win.left) * (100 - i)) / 100;
		int h = (hh * i + (win.bottom - win.top) * (100 - i)) / 100;
		SetWindowPos(hwnd, HWND_TOPMOST, (1920 - w) / 2, (1080 - h) / 2, w, h, SWP_SHOWWINDOW);
	}

	system("pause");
	return 0;
}