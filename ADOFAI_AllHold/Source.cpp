#include <iostream>
#include <string>
#include "Editor.h"
using namespace std;

int main() {
	Editor editor;
	cout << "[ ADOFAI Magic Square Maker ]" << endl << endl;

	cout << "Ư���� Ÿ�ϵ��� ������ bpm���� �ٲ��ݴϴ�" << endl;
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