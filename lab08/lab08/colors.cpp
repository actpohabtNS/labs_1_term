#include <iostream>
#include <string>

#include <stdio.h>		//
#include <stdlib.h>		// For setting
#include <windows.h>	// some pretty colors
#include <dos.h>

using namespace std;

const int BRDR_COLOR = 2, DEFAULT_COLOR = 7;

void SetColor(int ForgC)
{
	WORD wColor;

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	//We use csbi for the wAttributes word.
	if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
	{
		//Mask out all but the background attribute, and add in the forgournd color
		wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
	return;
}

void printc(string text, int color, string afterStr = "") {
	SetColor(color);
	cout << text;
	if (afterStr.length() != 0) cout << afterStr;
	SetColor(DEFAULT_COLOR);
}

void printc(int times, char sym, int color, string afterStr = "") {
	SetColor(color);
	for (int i = 0; i < times; i++) {
		cout << sym;
	}
	if (afterStr.length() != 0) cout << afterStr;
	SetColor(DEFAULT_COLOR);
}
