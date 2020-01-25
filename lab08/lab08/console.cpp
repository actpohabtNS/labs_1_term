#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include "console.h"

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void setCursor(short x, short y) {
	SetConsoleCursorPosition(hConsole, { x, y });
}

void setChar(short x, short y, char ch) {
	SetConsoleCursorPosition(hConsole, { x,y });
	cout << ch;
}

void clearLine(short y) {
	SetConsoleCursorPosition(hConsole, { 0,y });
	for (int i = 0; i < 100; i++) {
		cout << " ";
	}
}

void clearLine(short startPos, short endPos, short line) {
	for (int pos = startPos; pos < endPos; pos++) {
		setChar(pos, line, ' ');
	}
}

void clearLines(short y1, short y2) {
	for (short i = y1; i < y2; i++) {
		SetConsoleCursorPosition(hConsole, { 0, i });
		for (int i = 0; i < 100; i++) {
			cout << " ";
		}
	}
}

void selectOptionColumn(int options, int &option, bool &chosen) {

	char ch = _getch();
	if (49 <= ch && ch <= 57 && ch - 48 <= options) {
		option = ch - 48;
		chosen = true;
		}

	if (97 <= ch && ch <= 105 && ch - 96 <= options) {
		option = ch - 96;
		chosen = true;
		}

		switch (ch) {
		case 72:
		{// нажата клавиша вверх
			if (option > 1)
			{
				option--;
			}
			break;
		}
		case 80:
		{// нажата клавиша вниз
			if (option < options)
			{
				option++;
			}
			break;
		}
		case 8:
			option = 0;
			chosen = true;
			break;
		case 27:
			chosen = true;
			break;

		case 13:
			chosen = true;
			break;
		}
		//cout << option << " ";
}

void selectOptionRow(int options, int& option, bool& chosen) {

	char ch = _getch();
	switch (ch) {
	case 75:
	{// нажата клавиша влево
		if (option > 1)
		{
			option--;
		}
		break;
	}
	case 77:
	{// нажата клавиша вправо
		if (option < options)
		{
			option++;
		}
		break;
	}
	case 8:
		option = 0;
		chosen = true;
		break;
	case 27:
		chosen = true;
		break;

	case 13:
		chosen = true;
		break;
	}
	//cout << option << " ";
}