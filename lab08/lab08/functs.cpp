#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <windows.h>
#include <conio.h>

#include "colors.h"
#include "time.h"
#include "console.h"

using namespace std;

const int BRDR_COLOR = 2, DEFAULT_COLOR = 7, ACTIVE_OPTION_COLOR = 13, OPTION_COLOR = 5, MAIN_COLOR = 8,
BRIGHT_GREEN = 10, BRIGHT_RED = 12;
const int MARGIN = 10, MAIN_WIDTH = 100, EVENTS_PER_PAGE = 7;


void entry();
void schedule();
void manageSchedule();
void settingsMenu();
void printTime(short line, string mode, short margin = 0);

void addEvent();
void deleteFromSchedule();
void markEventDone();

struct Event {
	Date date;
	string place, event;
	int priority, lineNum;
};

struct Task {
	Date date;
	string task;
	bool done;
	int lineNum;
};

struct Settings {
	string name, sortedBy;
};

Settings settings = { "NULL", "date" };

Settings getSettings() {
	Settings sett;

	fstream file("..\\..\\files\\Planer\\settings.txt", ios_base::in | ios_base::out);

	file >> sett.name;
	file >> sett.sortedBy;

	if (!file.is_open()) {
		printc("[ ERROR ] ", 4); printc("File can't be opened!", DEFAULT_COLOR, "\n\n");
		sett.name = "";
	}

	if (sett.sortedBy == "" || sett.sortedBy == "\n") {
		file.close();
		fstream file("..\\..\\files\\Planer\\settings.txt", ios_base::app);
		file << "\ndate";
		sett.sortedBy = "date";
	}

	file.close();
	return sett;
}

void setSettings(Settings& sett, Settings newSett) {
	sett = newSett;

	fstream file("..\\..\\files\\Planer\\settings.txt", ios_base::out | ios_base::trunc);
	file << newSett.name << "\n";
	file << newSett.sortedBy;
	file.close();
}

void mergeVects(vector<Event> &main, vector<Event> side, string mode = "end") {
	if (mode == "start") reverse(main.begin(), main.end());

	for (Event evt : side) {
		main.push_back(evt);
	}

	if (mode == "start") reverse(main.begin(), main.end());
}

bool file_empty(std::fstream& pFile)
{
	return pFile.peek() == std::ifstream::traits_type::eof();
}

string tolower(string str) {
	string res;
	for (int sym = 0; sym < str.length(); sym++) {
		res += tolower(str[sym]);
	}
	return res;
}

void printFooter(int line) {
	setCursor(0, line);
	string phrase = "[Backspace] Return back";
	printc(0, ' ', MAIN_COLOR, phrase);

	printTime(line, "end");
}

void printGUI(int lines, bool side = true, bool toClearLines = true) {
	if (toClearLines) clearLines(1, 50);
	setCursor(0, 0);
	printc(MAIN_WIDTH / 2 - 4, '=', BRDR_COLOR);
	printc(" PlaneR ", MAIN_COLOR);
	printc(MAIN_WIDTH / 2 - 4, '=', BRDR_COLOR);


	if (side) printFooter(lines - 1);
	setCursor(0, lines);
	printc(MAIN_WIDTH, '=', BRDR_COLOR, "\n");
}

string getDayPhrase() {
	vector<string> phrases = { "Hello", "Hi", "What's up", "How is it going", "Hey" };

	srand(time(0));
	return phrases[1 + rand() % (phrases.size() - 1)];
}

string getUserName() {
	fstream file("..\\..\\files\\Planer\\settings.txt", ios_base::in | ios_base::out);

	string name;
	file >> name;

	if (!file.is_open()) {
		printc("[ ERROR ] ", 4); printc("File can't be opened!", DEFAULT_COLOR, "\n\n");
		name = "";
	}

	if (name == "" || name == "\n") {
		file.close();
		printc(MARGIN , ' ', DEFAULT_COLOR, "Enter your name: ");
		SetColor(10);
		cin >> name;
		cout << endl;
		fstream file("..\\..\\files\\Planer\\settings.txt", ios_base::app);
		file << name;
	}

	file.close();
	return name;
}

vector<string> split(string str, string del) {
	vector<string> res;
	int pos1 = 0, pos2 = 0;

	while (true) {
		pos2 = str.find(del, pos2);
		if (pos2 == string::npos) {
			if (str.length() != pos1) res.push_back(str.substr(pos1));
			break;
		}
		string substr;
		res.push_back(str.substr(pos1, pos2 - pos1));
		pos1 = pos2 + del.length();
		pos2 += del.length();
	}

	return res;
}

vector<Event> getEventsList(string search = "") {
	fstream file("..\\..\\files\\Planer\\events.txt", ios_base::in | ios_base::out);

	vector<Event> events, events1, events2, events3;

	string line;
	int lineNum = 0, priority = 0;

	while (getline(file, line)) {
		priority = 0;
		lineNum++;

		vector<string> data = split(line, "}{");
		int hour, minute, day, month;
		hour = stoi(split(data[0], ":")[0]);
		minute = stoi(split(data[0], ":")[1]);
		day = stoi(split(data[1], ":")[0]);
		month = stoi(split(data[1], ":")[1]);

		if (data.size() - 1 == 4) priority = stoi(data[4]);

		string event = "{" + to_string(priority) + "}" + getMonth(month) + " " + to_string(day) + " " + to_string(hour) + ":" + timeAdd0(minute) + " " + data[2] + " - " + data[3];
		if (tolower(event).find(tolower(search)) == string::npos) continue;


		if (settings.sortedBy == "date") events.push_back(Event{ {hour, minute, day, month}, data[2], data[3], priority, lineNum });
		else if (settings.sortedBy == "priority") {
			switch (priority) {
			case 0:
				events.push_back(Event{ {hour, minute, day, month}, data[2], data[3], priority, lineNum });
				break;

			case 1:
				events1.push_back(Event{ {hour, minute, day, month}, data[2], data[3], priority, lineNum });
				break;

			case 2:
				events2.push_back(Event{ {hour, minute, day, month}, data[2], data[3], priority, lineNum });
				break;

			case 3:
				events3.push_back(Event{ {hour, minute, day, month}, data[2], data[3], priority, lineNum });
				break;
			}
		}
	}

	file.close();

	if (settings.sortedBy == "priority") {
		mergeVects(events, events3, "start");
		mergeVects(events, events2, "start");
		mergeVects(events, events1, "start");
	}
	return events;
}

vector<Task> getTasksList(string search = "") {
	fstream file("..\\..\\files\\Planer\\tasks.txt", ios_base::in | ios_base::out);

	vector<Task> tasks;
	string line;
	int lineNum = 0;

	while (getline(file, line)) {
		lineNum++;

		vector<string> data = split(line, "}{");
		int hour = -1, minute = -1, day, month;
		day = stoi(split(data[0], ":")[0]);
		month = stoi(split(data[0], ":")[1]);
		bool done = false;
		if (data[2] == "true") done = true;

		cout << data.size() << " ";
		cout << data[2];

		string event = getMonth(month) + " " + to_string(day) + " "  + data[1];
		if (tolower(event).find(tolower(search)) == string::npos) continue;

		tasks.push_back(Task{ {hour, minute, day, month}, data[1], done, lineNum }); cout << "Tasks normal";
	}

	file.close();

	return tasks;
}

void getInput(string& input) {
	char ch;
	cin.get(ch);
	while (ch != '\n') {
		input += ch;
		cin.get(ch);
	}
}

void printTime(short line, string mode, short margin) {
	SYSTEMTIME st;
	GetSystemTime(&st);
	toLocalTime(st, 2);

	string time = timeAdd0(st.wHour) + ":" + timeAdd0(st.wMinute) + " " + getWeekDay(st.wDayOfWeek) + ", " +
		to_string(st.wDay) + " " + getMonth(st.wMonth) + " " + to_string(st.wYear);

	setCursor(0, line);
	int margin_time = 0;
	if (mode == "center") {
		margin_time = MAIN_WIDTH / 2 - time.length() / 2;
	}
	else if (mode == "start") {
		margin_time = 0;
	}
	else if (mode == "end") {
		margin_time = MAIN_WIDTH - time.length();
	}
	else if (mode == "margin") {
		margin_time = margin;
	}
	else {
		printc("[ ERROR ] : no such mode int [ Print Time ]", 12);
	}
	setCursor(margin_time, line);
	printc(time, MAIN_COLOR);
}

void updateTime(int line) {
	int s1, s2;

	SYSTEMTIME st;
	GetSystemTime(&st);
	s1 = st.wMinute;

	while (true) {
		SYSTEMTIME st;
		GetSystemTime(&st);
		s2 = st.wMinute;
		if (s1 != s2) {
			clearLine(line);
			setCursor(0, line);
			printTime(line, "center");
			s1 = s2;
		}
	}
}

void printEvents(string search = "", int topMargin = 10) {
	clearLines(topMargin-2, 50);
	setCursor(0, 0);

	vector<Event> events = getEventsList(search);

	int height = max(7 + topMargin, events.size() + topMargin);
	printGUI(height, true, false);
	setCursor(0, topMargin - 2);

	if (events.size() == 0) printc(MAIN_WIDTH / 6, ' ', DEFAULT_COLOR, "No such event!");

	for (short evNum = 0; evNum < events.size(); evNum++) {
		string time = "[" + to_string(events[evNum].lineNum) + "] " + getMonth(events[evNum].date.month) + " " + to_string(events[evNum].date.day) +
			", " + to_string(events[evNum].date.hour) + ":" + timeAdd0(events[evNum].date.minute);
		string evMain = events[evNum].place + " - " + events[evNum].event;

		int color = DEFAULT_COLOR;
		switch (events[evNum].priority) {
		case 0:
			color = DEFAULT_COLOR;
			break;
		case 1:
			color = 4; //4
			break;
		case 2:
			color = 6; //6
			break;
		case 3:
			color = 2; //2
			break;
		}

		printc(MAIN_WIDTH / 6, ' ', MAIN_COLOR, time);
		printc(" " + evMain + '\n', color);
	}
}

void printTasks(string search = "", int topMargin = 10) {
	clearLines(topMargin-2, 50);
	setCursor(0, 0);

	vector<Task> tasks = getTasksList(search);

	int height = max(7+topMargin, tasks.size() + topMargin);
	printGUI(height, true, false);
	setCursor(0, topMargin - 2);

	if (tasks.size() == 0) printc(MAIN_WIDTH / 6, ' ', DEFAULT_COLOR, "No such task!");

	for (short tNum = 0; tNum < tasks.size(); tNum++) {
		string time = "[" + to_string(tasks[tNum].lineNum) + "] " + getMonth(tasks[tNum].date.month) + " " + to_string(tasks[tNum].date.day);
		string evMain = tasks[tNum].task;
		printc(MAIN_WIDTH / 6, ' ', MAIN_COLOR, time);
		printc(" " + evMain + '\n', (tasks[tNum].done ? MAIN_COLOR : DEFAULT_COLOR));
	}
}

void deleteEvent(int evLine) {
	fstream file("..\\..\\files\\Planer\\events.txt");
	int lineNum = 1;
	string line, fileStr;

	while (getline(file, line)) {
		if (lineNum == evLine) {
			lineNum++;
			continue;
		}
		fileStr += line + "\n";
		lineNum++;
	}

	fileStr = fileStr.substr(0, fileStr.length() - 1);
	file.close();

	file.open("..\\..\\files\\Planer\\events.txt", ios_base::out | ios_base::trunc);
	file << fileStr;
	file.close();
}

void deleteTask(int tLine) {
	fstream file("..\\..\\files\\Planer\\tasks.txt");
	int lineNum = 1;
	string line, fileStr;

	while (getline(file, line)) {
		if (lineNum == tLine) {
			lineNum++;
			continue;
		}
		fileStr += line + "\n";
		lineNum++;
	}
	fileStr = fileStr.substr(0, fileStr.length() - 1);
	file.close();

	file.open("..\\..\\files\\Planer\\tasks.txt", ios_base::out | ios_base::trunc);
	file << fileStr;
	file.close();
}

void markTaskDone(int tLine) {
	fstream file("..\\..\\files\\Planer\\tasks.txt");
	int lineNum = 1;
	string line, fileStr;

	while (getline(file, line)) {
		if (lineNum == tLine) {
			vector<string> data = split(line, "}{");
			data[2] = "true";
			fileStr += data[0] + "}{" + data[1] + "}{" + data[2] + "\n";
		}
		else fileStr += line + "\n";

		lineNum++;
	}
	fileStr = fileStr.substr(0, fileStr.length() - 1);
	file.close();

	file.open("..\\..\\files\\Planer\\tasks.txt", ios_base::out | ios_base::trunc);
	file << fileStr;
	file.close();
}

void entry() {
	clearLines(1, 20);
	printGUI(13, false);

	setCursor(0, 2);

	string userName = getUserName(), phrase = getDayPhrase();

	settings = getSettings();

	clearLine(2);
	setCursor(0, 2);

	printc(MAIN_WIDTH / 2 - phrase.length()/2 - userName.length()/2 - 2, ' ' , DEFAULT_COLOR, phrase + ", ");
	printc(userName + "!\n\n\n", 10);

	setCursor(0, 5);
	printc(MAIN_WIDTH / 2 - 11, ' ', 8, "--->  ");
	printc("My schedule\n\n", ACTIVE_OPTION_COLOR); //6, 5
	printc(MAIN_WIDTH / 2 - 5, ' ', OPTION_COLOR, "Manage schedule\n\n");//8 - arrow
	printc(MAIN_WIDTH / 2 - 5, ' ', OPTION_COLOR, "Settings\n\n\n");//13


	printTime(12, "center");
	//updateTime(12);
	cout << endl << endl;

	bool chosen = false;
	int ans = 1;
	selectOptionColumn(3, ans, chosen);

	while (!chosen) {
		selectOptionColumn(3, ans, chosen);
		clearLine(5);
		clearLine(7);
		clearLine(9);
		setCursor(0, 5);

		switch (ans) {
		case 1:
			printc(MAIN_WIDTH / 2 - 11, ' ', 8, "--->  ");
			printc("My schedule\n\n", ACTIVE_OPTION_COLOR);
			printc(MAIN_WIDTH / 2 - 5, ' ', OPTION_COLOR, "Manage schedule\n\n");
			printc(MAIN_WIDTH / 2 - 5, ' ', OPTION_COLOR, "Settings\n\n\n");
			break;

		case 2:
			printc(MAIN_WIDTH / 2 - 5, ' ', OPTION_COLOR, "My schedule\n\n");
			printc(MAIN_WIDTH / 2 - 11, ' ', 8, "--->  ");
			printc("Manage schedule\n\n", ACTIVE_OPTION_COLOR);
			printc(MAIN_WIDTH / 2 - 5, ' ', OPTION_COLOR, "Settings\n\n\n");
			break;

		case 3:
			printc(MAIN_WIDTH / 2 - 5, ' ', OPTION_COLOR, "My schedule\n\n");
			printc(MAIN_WIDTH / 2 - 5, ' ', OPTION_COLOR, "Manage schedule\n\n");
			printc(MAIN_WIDTH / 2 - 11, ' ', 8, "--->  ");
			printc("Settings\n\n\n", ACTIVE_OPTION_COLOR);
			break;
		}

	}
	

	switch (ans) {
	case 1:
		schedule();
		break;

	case 2:
		manageSchedule();
		break;

	case 3:
		settingsMenu();
		break;
	}
	//cout << "Your choice: " << ans;
	setCursor(8, 16);
}

void schedule() {
	vector<Event> events = getEventsList();
	int height = max(17, events.size() + 10);

	clearLines(1, 20);
	printGUI(height);
	setCursor(0, 2);

	string phrase = "--[ Your schedule ]--";
	printc(MAIN_WIDTH / 2 - phrase.length()/2, ' ', ACTIVE_OPTION_COLOR, phrase + "\n\n"); //10, 11, 13

	phrase = "[ Events ]"; //length = 10;
	printc(MAIN_WIDTH / 4, ' ', ACTIVE_OPTION_COLOR, phrase);

	phrase = "[ Tasks ]";
	printc((MAIN_WIDTH / 4 - 10) * 2, ' ', OPTION_COLOR, phrase + "\n\n");

	phrase = "Search: ";
	printc(MAIN_WIDTH / 4, ' ', MAIN_COLOR, phrase + "\n\n");


	setCursor(0, 8);

	printEvents("");


	/*	[1]				[2]
		[searching]			*/
		
	bool chosen = false;
	int option = 1;
	bool searching = false;
	string search = "";

	while (!chosen) {
		char ch = _getch();
		switch (ch) {
		case 72:
		{// нажата клавиша вверх
			if (searching){
				option = 1;

				setCursor(0, 4);
				phrase = "[ Events ]";
				printc(MAIN_WIDTH / 4, ' ', ACTIVE_OPTION_COLOR, phrase);
				setCursor(0, 6);
				phrase = "Search: ";
				printc(MAIN_WIDTH / 4, ' ', MAIN_COLOR, phrase + "\n\n");

			}
			break;
		}
		case 80:
		{// нажата клавиша вниз
			if (option == 1){
				searching = true;

				setCursor(0, 6);
				phrase = "Search: ";
				printc(MAIN_WIDTH / 4, ' ', DEFAULT_COLOR, phrase + "\n\n");

				clearLine(MAIN_WIDTH / 4 + 8, 50, 6);
				setCursor(MAIN_WIDTH / 4 + 8, 6);
				getInput(search);

				printEvents(search);

				searching = false;
				search = "";
				setCursor(0, 6);
				phrase = "Search: ";
				printc(MAIN_WIDTH / 4, ' ', MAIN_COLOR, phrase + "\n\n");
			}

			if (option == 2) {
				searching = true;

				setCursor(0, 6);
				phrase = "Search: ";
				printc(MAIN_WIDTH / 4, ' ', DEFAULT_COLOR, phrase + "\n\n");

				clearLine(MAIN_WIDTH / 4 + 8, 50, 6);
				setCursor(MAIN_WIDTH / 4 + 8, 6);

				cin >> search;

				printTasks(search);

				searching = false;
				search = "";
				setCursor(0, 6);
				phrase = "Search: ";
				printc(MAIN_WIDTH / 4, ' ', MAIN_COLOR, phrase + "\n\n");
			}
			break;
		}
		case 75:
		{// нажата клавиша влево
			if (option == 2 && !searching){
				option = 1;

				setCursor(0, 4);
				phrase = "[ Events ]";
				printc(MAIN_WIDTH / 4, ' ', ACTIVE_OPTION_COLOR, phrase);
				phrase = "[ Tasks ]";
				printc((MAIN_WIDTH / 4 - 10) * 2, ' ', OPTION_COLOR, phrase + "\n\n");

				printEvents();
				search = "";
				clearLine(6);
				setCursor(0, 6);
				phrase = "Search: ";
				printc(MAIN_WIDTH / 4, ' ', MAIN_COLOR, phrase + "\n\n");
			}
			break;
		}
		case 77:
		{// нажата клавиша вправо
			if (option == 1 && !searching){
				option = 2;

				setCursor(0, 4);
				phrase = "[ Events ]";
				printc(MAIN_WIDTH / 4, ' ', OPTION_COLOR, phrase);
				phrase = "[ Tasks ]";
				printc((MAIN_WIDTH / 4 - 10) * 2, ' ', ACTIVE_OPTION_COLOR, phrase + "\n\n");

				printTasks();
				search = "";
				clearLine(6);
				setCursor(0, 6);
				phrase = "Search: ";
				printc(MAIN_WIDTH / 4, ' ', MAIN_COLOR, phrase + "\n\n");
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
	}

	switch (option) {
	case 0:
		entry();
		break;
	}
}

void manageSchedule() {
	clearLines(1, 15);
	printGUI(13);
	setCursor(0, 2);

	string phrase = "--[ Manage schedule ]--";
	printc(MAIN_WIDTH / 2 - phrase.length()/2, ' ', ACTIVE_OPTION_COLOR, phrase + "\n\n\n");
	
	setCursor(0, 5);
	printc(MAIN_WIDTH / 2 - 11, ' ', 8, "--->  ");
	printc("Add new\n\n", ACTIVE_OPTION_COLOR); //6, 5
	printc(MAIN_WIDTH / 2 - 5, ' ', 5, "Delete\n\n"); //8 - arrow
	printc(MAIN_WIDTH / 2 - 5, ' ', 5, "Mark task done\n\n\n"); //13


	
	bool chosen = false;
	int ans = 1;
	selectOptionRow(1, ans, chosen);

	while (!chosen) {
		selectOptionColumn(3, ans, chosen);

		
		clearLine(5);
		clearLine(7);
		clearLine(9);
		setCursor(0, 5);

		switch (ans) {
		case 1:
			printc(MAIN_WIDTH / 2 - 11, ' ', 8, "--->  ");
			printc("Add new\n\n", ACTIVE_OPTION_COLOR);
			printc(MAIN_WIDTH / 2 - 5, ' ', 5, "Delete\n\n");
			printc(MAIN_WIDTH / 2 - 5, ' ', 5, "Mark task done\n\n\n");
			break;

		case 2:
			printc(MAIN_WIDTH / 2 - 5, ' ', 5, "Add new\n\n");
			printc(MAIN_WIDTH / 2 - 11, ' ', 8, "--->  ");
			printc("Delete\n\n", ACTIVE_OPTION_COLOR);
			printc(MAIN_WIDTH / 2 - 5, ' ', 5, "Mark task done\n\n\n");
			break;

		case 3:
			printc(MAIN_WIDTH / 2 - 5, ' ', 5, "Add new\n\n");
			printc(MAIN_WIDTH / 2 - 5, ' ', 5, "Delete\n\n");
			printc(MAIN_WIDTH / 2 - 11, ' ', 8, "--->  ");
			printc("Mark task done\n\n\n", ACTIVE_OPTION_COLOR);
			break;
		}

	
	}

	switch (ans) {
	case 0:
		entry();
		break; 

	case 1:
		addEvent();
		break;

	case 2:
		deleteFromSchedule();
		break;

	case 3:
		markEventDone();
		break;
	}

}

void settingsMenu() {
	clearLines(1, 15);
	printGUI(13);
	setCursor(0, 2);

	string phrase = "--[ Settings ]--";
	printc(MAIN_WIDTH / 2 - phrase.length()/2, ' ', ACTIVE_OPTION_COLOR, phrase + "\n\n\n");


	setCursor(0, 4);
	printc(MAIN_WIDTH / 5, ' ', MAIN_COLOR, "[1] User name: ");
	printc(settings.name, BRIGHT_GREEN);

	setCursor(0, 6);
	printc(MAIN_WIDTH / 5, ' ', MAIN_COLOR, "[2] Events / Tasks are sorted by: ");

	bool chosen2 = false;
	int ans2;

	if (settings.sortedBy == "date") {
		printc(5,' ', ACTIVE_OPTION_COLOR, "date");
		printc(5, ' ', OPTION_COLOR, "priority");

		ans2 = 1;
	}
	else {
		printc(5, ' ', OPTION_COLOR, "date");
		printc(5, ' ', ACTIVE_OPTION_COLOR, "priority");

		ans2 = 2;
	}


	
	bool chosen = false;
	int ans = 1;
	string newName, newSortedBy;

	bool chosen1 = false;
	int ans1 = 2;
	

	while (!chosen) {
		selectOptionColumn(2, ans, chosen);

		switch (ans) {
		case 1:
			chosen = false;
			clearLine(9);
			clearLine(25, 70, 12);

			clearLine(4);
			setCursor(0, 4);
			printc(MAIN_WIDTH / 5, ' ', DEFAULT_COLOR, "[1] User name: ");
			SetColor(BRIGHT_GREEN);
			getInput(newName);

			setCursor(0, 9);
			phrase = "Are you sure? ";
			printc(MAIN_WIDTH / 2 - phrase.length(), ' ', DEFAULT_COLOR, phrase);
			printc(5, ' ', MAIN_COLOR, "Yes");  printc(10, ' ', BRIGHT_RED, "No");

			

			while (!chosen1) {
				selectOptionRow(2, ans1, chosen1);

				switch (ans1) {
				case 1:
					setCursor(MAIN_WIDTH / 2, 9);
					printc(5, ' ', BRIGHT_GREEN, "Yes");  printc(10, ' ', MAIN_COLOR, "No");
					break;

				case 2:
					setCursor(MAIN_WIDTH / 2, 9);
					printc(5, ' ', MAIN_COLOR, "Yes");  printc(10, ' ', BRIGHT_RED, "No");
					break;
				}
			}

			switch (ans1) {
			case 1:
				setSettings(settings, Settings{ newName, settings.sortedBy });
				phrase = "Settings [name] updated successfuly!";
				setCursor(MAIN_WIDTH / 2 - phrase.length() / 2, 12);
				printc(phrase, BRIGHT_GREEN);
				break;
			case 2:
				phrase = "Updating settings [name] cancelled!";
				setCursor(MAIN_WIDTH / 2 - phrase.length() / 2, 12);
				printc(phrase, BRIGHT_RED);

				setCursor(0, 4);
				printc(MAIN_WIDTH / 5, ' ', MAIN_COLOR, "[1] User name: ");
				printc(settings.name, BRIGHT_GREEN);
				chosen1 = false;
				break;
			}

			break;

		case 2:
			chosen = false;

			clearLine(9);
			clearLine(25, 70, 12);

			setCursor(0, 6);
			printc(MAIN_WIDTH / 5, ' ', DEFAULT_COLOR, "[2] Events / Tasks are sorted by: ");

			while (!chosen2) {
				selectOptionRow(2, ans2, chosen2);

				setCursor(0, 6);
				printc(MAIN_WIDTH / 5, ' ', DEFAULT_COLOR, "[2] Events / Tasks are sorted by: ");

				switch (ans2) {
				case 1:

					printc(5, ' ', ACTIVE_OPTION_COLOR, "date");
					printc(5, ' ', OPTION_COLOR, "priority");
					break;
				case 2:

					printc(5, ' ', OPTION_COLOR, "date");
					printc(5, ' ', ACTIVE_OPTION_COLOR, "priority");
					break;
				}
			}

			switch (ans2) {
			case 1:
				newSortedBy = "date";
				break;
			case 2:
				newSortedBy = "priority";
				break;
			}

			setCursor(0, 9);
			phrase = "Are you sure? ";
			printc(MAIN_WIDTH / 2 - phrase.length(), ' ', DEFAULT_COLOR, phrase);
			printc(5, ' ', MAIN_COLOR, "Yes");  printc(10, ' ', BRIGHT_RED, "No");



			while (!chosen1) {
				selectOptionRow(2, ans1, chosen1);

				switch (ans1) {
				case 1:
					setCursor(MAIN_WIDTH / 2, 9);
					printc(5, ' ', BRIGHT_GREEN, "Yes");  printc(10, ' ', MAIN_COLOR, "No");
					break;

				case 2:
					setCursor(MAIN_WIDTH / 2, 9);
					printc(5, ' ', MAIN_COLOR, "Yes");  printc(10, ' ', BRIGHT_RED, "No");
					break;
				}
			}

			switch (ans1) {
			case 1:
				setSettings(settings, Settings{ settings.name, newSortedBy });
				phrase = "Settings [sortion] updated successfuly!";
				setCursor(MAIN_WIDTH / 2 - phrase.length() / 2, 12);
				printc(phrase, BRIGHT_GREEN);
				break;
			case 2:
				phrase = "Updating settings [sortion] cancelled!";
				setCursor(MAIN_WIDTH / 2 - phrase.length() / 2, 12);
				printc(phrase, BRIGHT_RED);
				break;
			}

			break;

		}
		
	}

	switch (ans) {
	case 0:
		entry();
		break;
	}
}

void addEvent() {
	clearLines(1, 17);
	printGUI(16);
	setCursor(0, 2);

	string phrase = "--[ Add event ]--";
	printc(MAIN_WIDTH / 2 - phrase.length() / 2, ' ', ACTIVE_OPTION_COLOR, phrase + "\n\n");

	string time, dayMonth, place, event, prioStr;
	bool correctAnswers = false;
	int prio = 0;

	setCursor(0, 4);
	printc(20, ' ', MAIN_COLOR, "[1] Priority (1-3): ");
	setCursor(0, 6);
	printc(3, ' ', MAIN_COLOR, "(no time = task) [2] Time (hh:mm): ");
	setCursor(0, 8);
	printc(20, ' ', MAIN_COLOR, "[3] Day (dd:mm): ");
	setCursor(0, 10);
	printc(2, ' ', MAIN_COLOR, "(no place = task) [4] Place: ");
	setCursor(0, 12);
	printc(20, ' ', MAIN_COLOR, "[5] Event: ");

	bool chosen = false;
	int ans = 1;

	while ((dayMonth.empty() || event.empty()) || time.empty() != place.empty()) {
		setCursor(0, 16);
		selectOptionColumn(5, ans, chosen);
		switch (ans) {
		case 0:
			manageSchedule();
			break;
		
		case 1:
			if (prio != 0) break;
			setCursor(40, 4);
			getline(cin, prioStr);
			prio = stoi(prioStr);
			break;
		case 2:
			if (!time.empty()) break;
			setCursor(38, 6);
			getline(cin, time);
			break;
		case 3:
			if (!dayMonth.empty()) break;
			setCursor(37, 8);
			getline(cin, dayMonth);
			break;
		case 4:
			if (!place.empty()) break;
			setCursor(31, 10);
			getline(cin, place);
			break;
		case 5:
			if (!event.empty()) break;
			setCursor(31, 12);
			getline(cin, event);
			break;
		}
	}

	bool isEvent = true;
	if (time.empty()) isEvent = false;

	// TODO: add BETTER correct check
	int hour = -1, minute = -1, day, month;
	if (isEvent) {
		hour = stoi(split(time, ":")[0]);
		minute = stoi(split(time, ":")[1]);
	}
	day = stoi(split(dayMonth, ":")[0]);
	month = stoi(split(dayMonth, ":")[1]);

	setCursor(37, 12); //38, 13
	if (0 >= day || day > 31) printc("Wrong day number! (0 < day <= 31)", DEFAULT_COLOR);
	else if (0 >= month || month > 12) printc("Wrong month number! (0 < month <= 12)", DEFAULT_COLOR);
	else if (-1 > hour || hour > 24) printc("Wrong hour number! (0 < hour <= 24)", DEFAULT_COLOR);
	else if (-1 > minute || minute > 60) printc("Wrong minute number! (0 <= minute <= 59)", DEFAULT_COLOR);
	else if (prio < 0 || prio > 3) printc("Wrong priority number! (0 <= priority <= 3)", DEFAULT_COLOR);
	else correctAnswers = true;

	if (!correctAnswers) {
		setCursor(27, 12);
		printc("[ ERROR ] Wrong answers", 4);
	}


	if (correctAnswers) {
		fstream file;

		if (!isEvent) file.open("..\\..\\files\\Planer\\tasks.txt");
		else file.open("..\\..\\files\\Planer\\events.txt");

		string line1, line2, newFileStr = "";

		string newEvent = time + "}{" + dayMonth + "}{" + place + "}{" + event;
		if (prio != 0) newEvent += "}{" + to_string(prio);

		string newTask = dayMonth + "}{" + event + "}{false";

		getline(file, line1);
		getline(file, line2);

		int hour1 = -1, minute1 = -1, day1, month1, hour2 = -1, minute2 = -1, day2, month2;

		if (line1.empty()) { //Empty file => just add new line
			if (isEvent) newFileStr = newEvent;
			else newFileStr = newTask;
		}
		else if (line2.empty()){ //File with 1 line => compare and add
			vector<string> data = split(line1, "}{");

			if (isEvent) {
				hour1 = stoi(split(data[0], ":")[0]);
				minute1 = stoi(split(data[0], ":")[1]);
				day1 = stoi(split(data[1], ":")[0]);
				month1 = stoi(split(data[1], ":")[1]);
			}
			else {
				day1 = stoi(split(data[0], ":")[0]);
				month1 = stoi(split(data[0], ":")[1]);
			}


			Date newDate = { hour, minute, day, month };
			Date date1 = { hour1, minute1, day1, month1 };

			if (compareDate(newDate, date1) == 1) {
				newFileStr += line1 + "\n";
				if (isEvent) newFileStr += newEvent;
				else newFileStr += newTask;
			}
			else {
				if (isEvent) newFileStr += newEvent;
				else newFileStr += newTask;
				newFileStr += "\n" + line1;
			}
		}
		else { //File with 2+ line => compare and add + add remain
			bool newLineAdded = false;
			do {
				vector<string> data1 = split(line1, "}{");
				day1 = stoi(split(data1[1], ":")[0]);
				month1 = stoi(split(data1[1], ":")[1]);

				if (isEvent) {
					hour1 = stoi(split(data1[0], ":")[0]);
					minute1 = stoi(split(data1[0], ":")[1]);
				}

				vector<string> data2 = split(line2, "}{");
				day2 = stoi(split(data2[1], ":")[0]);
				month2 = stoi(split(data2[1], ":")[1]);

				if (isEvent) {
					hour2 = stoi(split(data2[0], ":")[0]);
					minute2 = stoi(split(data2[0], ":")[1]);
				}

				Date newDate = {hour, minute, day, month};
				Date date1 = {hour1, minute1, day1, month1};
				Date date2 = {hour2, minute2, day2, month2};

				setCursor(0, 18);

				if (compareDate(date1, newDate) == 1) { // newDate < date1 < date2
					//cout << "newDate < date1 < date2" << endl;
					if (isEvent) newFileStr += newEvent + "\n";
					else newFileStr += newTask + "\n";
					newFileStr += line1 + "\n" + line2 + "\n";
					newLineAdded = true;
					break;
				}

				if (compareDate(newDate, date1) == 1 && compareDate(date2, newDate) >= 0) { //date1 < newDate =< date2
					//cout << "date1 < newDate =< date2" << endl;
					newFileStr += line1 + "\n";
					if (isEvent) newFileStr += newEvent + "\n";
					else newFileStr += newTask + "\n";
					newFileStr += line2 + "\n";
					newLineAdded = true;
					break;
				}


				newFileStr += line1 + "\n";
				line1 = line2;

			} while (getline(file, line2));


			if (!newLineAdded) {
				//cout << "newLine hasn't been added. Adding..." << endl;
				newFileStr += line2 + "\n";
				if (isEvent) newFileStr += newEvent + "\n";
				else newFileStr += newTask + "\n";
			}

			while (getline(file, line1)) {
				//cout << "Adding last line to newFileStr" << endl;
				newFileStr += line1 + "\n";
			}

			newFileStr = newFileStr.substr(0, newFileStr.length() - 1);

		}
		file.close();

		if (!isEvent) file.open("..\\..\\files\\Planer\\tasks.txt", ios_base::out | ios_base::trunc);
		else file.open("..\\..\\files\\Planer\\events.txt", ios_base::out | ios_base::trunc);

		file << newFileStr;

		setCursor(38, 13);
		if (isEvent) printc("Event added successfuly!", 12);
		else printc("Task added successfuly!", 12);

		file.close();
	}

	chosen = false;

	while (!chosen) {
		selectOptionColumn(1, ans, chosen);
	}

	switch (ans) {
	case 0:
		manageSchedule();
		break;
	}
}

void deleteFromSchedule(){
	vector<Event> events = getEventsList();
	int height = max(21, events.size() + 14);

	clearLines(1, 30);
	printGUI(height);
	setCursor(0, 2);

	string phrase = "--[ Delete ]--";
	printc(MAIN_WIDTH / 2 - phrase.length() / 2, ' ', ACTIVE_OPTION_COLOR, phrase + "\n\n"); //10, 11, 13

	phrase = "[ Num ] to delete: ";
	printc(MAIN_WIDTH / 2 - phrase.length()/2, ' ', MAIN_COLOR, phrase);

	setCursor(0, 8);

	phrase = "[ Events ]"; //length = 10;
	printc(MAIN_WIDTH / 4, ' ', ACTIVE_OPTION_COLOR, phrase);

	phrase = "[ Tasks ]";
	printc((MAIN_WIDTH / 4 - 10) * 2, ' ', OPTION_COLOR, phrase + "\n\n");

	phrase = "Search: ";
	printc(MAIN_WIDTH / 4, ' ', MAIN_COLOR, phrase + "\n\n");


	setCursor(0, 12);

	printEvents("", 14);


	/*			[C1]
		[R1]			[R2]
		[C2]				
								*/

	bool chosen = false;
	int optionRow = 1;
	int optionColumn = 0;
	string search = "";
	string lineToDelStr = "";
	int lineToDel = -1;

	while (!chosen) {
		char ch = _getch();
		switch (ch) {
		case 72:
		{// нажата клавиша вверх
			if (optionColumn == 2) {
				optionRow = 1;

				setCursor(0, 8);
				phrase = "[ Events ]";
				printc(MAIN_WIDTH / 4, ' ', ACTIVE_OPTION_COLOR, phrase);
				setCursor(0, 10);
				phrase = "Search: ";
				printc(MAIN_WIDTH / 4, ' ', MAIN_COLOR, phrase + "\n\n");

			}

			if (optionColumn == 0) {
				optionColumn = 1;

				setCursor(0, 4);
				phrase = "[ Num ] to delete: ";
				printc(MAIN_WIDTH / 2 - phrase.length() / 2, ' ', DEFAULT_COLOR, phrase);

				while (lineToDelStr == "") {
					setCursor(MAIN_WIDTH / 2 + phrase.length() / 2 + 1, 4);
					getInput(lineToDelStr);
				}

				lineToDel = stoi(lineToDelStr);

				setCursor(0, 6);
				phrase = "Are you sure? ";
				printc(MAIN_WIDTH / 2 - phrase.length(), ' ', DEFAULT_COLOR, phrase);
				printc(5, ' ', MAIN_COLOR, "Yes");  printc(10, ' ', BRIGHT_RED, "No");

				bool chosen = false;
				int ans = 2;

				while (!chosen) {
					selectOptionRow(2, ans, chosen);

					switch (ans) {
					case 1:
						setCursor(MAIN_WIDTH / 2, 6);
						printc(5, ' ', BRIGHT_GREEN, "Yes");  printc(10, ' ', MAIN_COLOR, "No");
						break;

					case 2:
						setCursor(MAIN_WIDTH / 2, 6);
						printc(5, ' ', MAIN_COLOR, "Yes");  printc(10, ' ', BRIGHT_RED, "No");
						break;
					}
				}

				switch (ans) {
				case 1:
					if (optionRow == 1) {
						deleteEvent(lineToDel);
						printEvents(search, 14);
					}
					else {
						deleteTask(lineToDel);
						printTasks(search, 14);
					}
					break;

				case 2:

					break;
				}
			}

			clearLine(4);
			setCursor(0, 4);
			phrase = "[ Num ] to delete: ";
			printc(MAIN_WIDTH / 2 - phrase.length() / 2, ' ', MAIN_COLOR, phrase);

			clearLine(6);
			optionColumn = 0;

			lineToDelStr = "";

			break;
		}
		case 80:
		{// нажата клавиша вниз
			if (optionRow == 1) {
				optionColumn = 1;

				setCursor(0, 10);
				phrase = "Search: ";
				printc(MAIN_WIDTH / 4, ' ', DEFAULT_COLOR, phrase + "\n\n");

				clearLine(MAIN_WIDTH / 4 + 8, 50, 10);
				setCursor(MAIN_WIDTH / 4 + 8, 10);
				getInput(search);

				printEvents(search, 14);

				optionColumn = 0;
				search = "";
				setCursor(0, 10);
				phrase = "Search: ";
				printc(MAIN_WIDTH / 4, ' ', MAIN_COLOR, phrase + "\n\n");
			}

			if (optionRow == 2) {
				optionColumn = 2;

				setCursor(0, 10);
				phrase = "Search: ";
				printc(MAIN_WIDTH / 4, ' ', DEFAULT_COLOR, phrase + "\n\n");

				clearLine(MAIN_WIDTH / 4 + 8, 50, 10);
				setCursor(MAIN_WIDTH / 4 + 8, 10);

				cin >> search;

				printTasks(search, 14);

				optionColumn = 0;
				search = "";
				setCursor(0, 10);
				phrase = "Search: ";
				printc(MAIN_WIDTH / 4, ' ', MAIN_COLOR, phrase + "\n\n");
			}
			break;
		}
		case 75:
		{// нажата клавиша влево
			if (optionRow == 2 && optionColumn == 0) {
				optionRow = 1;

				setCursor(0, 8);
				phrase = "[ Events ]";
				printc(MAIN_WIDTH / 4, ' ', ACTIVE_OPTION_COLOR, phrase);
				phrase = "[ Tasks ]";
				printc((MAIN_WIDTH / 4 - 10) * 2, ' ', OPTION_COLOR, phrase + "\n\n");

				printEvents("", 14);
				search = "";
				clearLine(10);
				setCursor(0, 10);
				phrase = "Search: ";
				printc(MAIN_WIDTH / 4, ' ', MAIN_COLOR, phrase + "\n\n");
			}
			break;
		}
		case 77:
		{// нажата клавиша вправо
			if (optionRow == 1 && optionColumn == 0) {
				optionRow = 2;

				setCursor(0, 8);
				phrase = "[ Events ]";
				printc(MAIN_WIDTH / 4, ' ', OPTION_COLOR, phrase);
				phrase = "[ Tasks ]";
				printc((MAIN_WIDTH / 4 - 10) * 2, ' ', ACTIVE_OPTION_COLOR, phrase + "\n\n");

				printTasks("", 14);
				search = "";
				clearLine(10);
				setCursor(0, 10);
				phrase = "Search: ";
				printc(MAIN_WIDTH / 4, ' ', MAIN_COLOR, phrase + "\n\n");
			}
			break;
		}

		case 8:
			optionRow = 0;
			chosen = true;
			break;
		case 27:
			chosen = true;
			break;

		case 13:
			chosen = true;
			break;
		}
	}

	switch (optionRow) {
	case 0:
		manageSchedule();
		break;
	}
}

void markEventDone() {
	vector<Task> tasks = getTasksList();
	int height = max(21, tasks.size() + 14);

	clearLines(1, 30);
	printGUI(height);
	setCursor(0, 2);

	string phrase = "--[ Mark task done ]--";
	printc(MAIN_WIDTH / 2 - phrase.length() / 2, ' ', ACTIVE_OPTION_COLOR, phrase + "\n\n"); //10, 11, 13

	phrase = "[ Num ] to mark done: ";
	printc(MAIN_WIDTH / 2 - phrase.length() / 2, ' ', MAIN_COLOR, phrase);

	setCursor(0, 8);

	phrase = "[ Tasks ]"; //length = 10;
	printc(MAIN_WIDTH / 2 - phrase.length()/2, ' ', ACTIVE_OPTION_COLOR, phrase + "\n\n");

	phrase = "Search: ";
	printc(MAIN_WIDTH / 4, ' ', MAIN_COLOR, phrase + "\n\n");


	setCursor(0, 12);

	printTasks("", 14);

	/*			[C1]
			   [MAIN]
				[C2]	*/

	bool chosen = false;
	int optionColumn = 0;
	string search = "";
	string lineToMarkStr = "";
	int lineToMark = -1;

	while (!chosen) {
		char ch = _getch();
		switch (ch) {
		case 72:
		{// нажата клавиша вверх
			
				optionColumn = 1;

				setCursor(0, 4);
				phrase = "[ Num ] to mark done: ";
				printc(MAIN_WIDTH / 2 - phrase.length() / 2, ' ', DEFAULT_COLOR, phrase);

				while (lineToMarkStr == "") {
					setCursor(MAIN_WIDTH / 2 + phrase.length() / 2 + 1, 4);
					getInput(lineToMarkStr);
				}

				lineToMark = stoi(lineToMarkStr);

				setCursor(0, 6);
				phrase = "Are you sure? ";
				printc(MAIN_WIDTH / 2 - phrase.length(), ' ', DEFAULT_COLOR, phrase);
				printc(5, ' ', MAIN_COLOR, "Yes");  printc(10, ' ', BRIGHT_RED, "No");

				bool chosen = false;
				int ans = 2;

				while (!chosen) {
					selectOptionRow(2, ans, chosen);

					switch (ans) {
					case 1:
						setCursor(MAIN_WIDTH / 2, 6);
						printc(5, ' ', BRIGHT_GREEN, "Yes");  printc(10, ' ', MAIN_COLOR, "No");
						break;

					case 2:
						setCursor(MAIN_WIDTH / 2, 6);
						printc(5, ' ', MAIN_COLOR, "Yes");  printc(10, ' ', BRIGHT_RED, "No");
						break;
					}
				}

				switch (ans) {
				case 1:
					markTaskDone(lineToMark);
					printTasks(search, 14);
					break;

				case 2:

					break;
				}
			

			clearLine(4);
			setCursor(0, 4);
			phrase = "[ Num ] to mark done: ";
			printc(MAIN_WIDTH / 2 - phrase.length() / 2, ' ', MAIN_COLOR, phrase);

			clearLine(6);
			optionColumn = 0;

			lineToMarkStr = "";

			break;
		}
		case 80:
		{// нажата клавиша вниз
				optionColumn = 2;

				setCursor(0, 10);
				phrase = "Search: ";
				printc(MAIN_WIDTH / 4, ' ', DEFAULT_COLOR, phrase + "\n\n");

				clearLine(MAIN_WIDTH / 4 + 8, 50, 10);
				setCursor(MAIN_WIDTH / 4 + 8, 10);
				getInput(search);

				printTasks(search, 14);

				optionColumn = 0;
				search = "";
				setCursor(0, 10);
				phrase = "Search: ";
				printc(MAIN_WIDTH / 4, ' ', MAIN_COLOR, phrase + "\n\n");
			break;
		}

		case 8:
			optionColumn = 0;
			chosen = true;
			break;
		case 27:
			chosen = true;
			break;

		case 13:
			chosen = true;
			break;
		}
	}

	switch (optionColumn) {
	case 0:
		manageSchedule();
		break;
	}
}