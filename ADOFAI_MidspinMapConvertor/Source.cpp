#include <iostream>
#include "Convertor.h"
using namespace std;
int main() {
	Convertor convertor;
	cout << "[ADOFAI MidSpin Map Convertor]" << endl << endl;
	cout << "���� ��� Ÿ���� �̵彺������ �����մϴ�" << endl;
	cout << "Developer : CLiF" << endl;
	cout << "Version : 1.0" << endl << endl;
	cout << "ADOFAI File Path : ";
	string path;
	cin >> path;
	if (convertor.Open(path)) {
		return 1;
	}
	convertor.ChangeFile();
	convertor.Save(path);
	cout << endl;
	cout << "Completed!!";
	system("pause");
	return 0;
}