#include <iostream>
#include "Editor.h"
using namespace std;
int main() {
	Editor editor;
	cout << "[ADOFAI All Simultaneously Hit Convertor]" << endl << endl;
	cout << "���� ��� Ÿ���� 15�� ��Ÿ Ÿ�Ϸ� �����մϴ�" << endl;
	cout << "Developer : CLiF" << endl << endl;
	cout << "Version : 1.0" << endl << endl;

	cout << "ADOFAI File Path : ";
	string path;
	cin >> path;
	cout << "Converting . . ." << endl;
	editor.Open(path);
	//editor.ChangeFile();
	editor.ChangeFileOther();
	cout << "Saving . . ." << endl;
	editor.Save(path);
	cout << "Completed!!" << endl;
	system("pause");
	return 0;
}