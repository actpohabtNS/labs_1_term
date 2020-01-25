#pragma once
#include <iostream>
#include <ctime>
#include <string>
#include <windows.h>

using namespace std;

struct Date {
	int hour, minute, day, month;
};

string getWeekDay(int day);

string getMonth(int month);

string timeAdd0(int time);

int getDaysOfMonth(int month, int year);

void toLocalTime(SYSTEMTIME& st, int shift);

int compareDate(Date date1, Date date2);