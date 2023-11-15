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

	cout << "BPM : ";
	double bpm;
	cin >> bpm;
	editor.SetBPM(bpm);

	cout << "Begin Tile Number : ";
	double begin;
	cin >> begin;
	cout << "End Tile Number : ";
	double end;
	cin >> end;
	editor.SetRange(begin, end);

	cout << endl << "Transforming . . ." << endl;
	editor.ChangeFile();
	cout << endl;
	editor.Save(loc);
	cout << "Completed!" << endl;
	system("pause");
	return 0;
}