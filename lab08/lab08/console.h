#pragma once
#include <windows.h>
#include <string>

void setCursor(short x, short y);

void setChar(short x, short y, char ch);

void clearLine(short y);

void clearLine(short startPos, short endPos, short line);

void clearLines(short y1, short y2);

void selectOptionColumn(int options, int& option, bool& chosen);

void selectOptionRow(int options, int& option, bool& chosen);