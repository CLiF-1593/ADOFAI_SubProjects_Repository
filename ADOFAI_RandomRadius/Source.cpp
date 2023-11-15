#include <iostream>
#include <string>
#include "Editor.h"
using namespace std;

int main() {
	Editor editor;
	cout << "[ ADOFAI Magic Square Maker ]" << endl << endl;

	cout << "특정한 타일들을 일정한 bpm으로 바꿔줍니다" << endl;
	cout << "Developer : CLiF" << endl;
	cout << "Version : 1.0" << endl << endl;

	cout << "adofai file location : ";
	string loc;
	cin >> loc;
	editor.Open(loc);

	cout << endl << "Transforming . . ." << endl;
	editor.ChangeFile();
	cout << endl;
	editor.Save(loc);
	cout << "Completed!" << endl;
	system("pause");
	return 0;
}