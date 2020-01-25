#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <windows.h>

using namespace std;

struct Date {
	int hour, minute, day, month;
};

string getWeekDay(int day) {
	switch (day) {
	case 1:
		return "Monday";

	case 2:
		return "Tuesday";

	case 3:
		return "Wednesday";

	case 4:
		return "Thursday";

	case 5:
		return "Friday";

	case 6:
		return "Saturday";

	case 0:
		return "Sunday";

	default:
		return "NO SUCH DAY!";
	}
}

string getMonth(int month) {
	switch (month) {
	case 1:
		return "Jan";

	case 2:
		return "Feb";

	case 3:
		return "Mar";

	case 4:
		return "Apr";

	case 5:
		return "May";

	case 6:
		return "Jun";

	case 7:
		return "Jul";

	case 8:
		return "Aug";

	case 9:
		return "Sep";

	case 10:
		return "Oct";

	case 11:
		return "Nov";

	case 12:
		return "Dec";

	default:
		return "NO SUCH MONTH";
	}
}

string timeAdd0(int time) {
	if (time < 10) return "0" + to_string(time);
	else return to_string(time);
}

int getDaysOfMonth(int month, int year) {
	switch (month) {
	case 1:
		return 31;

	case 2:
		if (year % 4 == 0) return 29;
		else return 28;

	case 3:
		return 31;

	case 4:
		return 30;

	case 5:
		return 31;

	case 6:
		return 30;

	case 7:
		return 31;

	case 8:
		return 31;

	case 9:
		return 30;

	case 10:
		return 31;

	case 11:
		return 30;

	case 12:
		return 31;

	default:
		return -1;
	}
}

void toLocalTime(SYSTEMTIME& st, int shift) {
	shift %= 24;

	if (st.wHour + shift > 23) {
		st.wHour += shift - 24;
		if (st.wDay + 1 > getDaysOfMonth(st.wMonth, st.wYear)) {
			st.wDay = 1;
			if (st.wMonth == 12) {
				st.wMonth = 1;
				st.wYear++;
			}
			else st.wMonth++;
		}
		else st.wDay++;
		if (st.wDayOfWeek == 7) st.wDayOfWeek = 1;
		else st.wDayOfWeek++;
	}
	else st.wHour += shift;
}

int compareDate(Date date1, Date date2) {
	if (date1.month != date2.month)
		return (date1.month - date2.month > 0) ? 1 : -1;

	if (date1.day != date2.day)
		return (date1.day - date2.day > 0) ? 1 : -1;

	if (date1.hour != date2.hour)
		return (date1.hour - date2.hour > 0) ? 1 : -1;

	if (date1.minute != date2.minute)
		return (date1.minute - date2.minute > 0) ? 1 : -1;

	return 0;
}