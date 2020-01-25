#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int entries(string str, string substr) {
	int res = 0, pos = 0;

	while (true) {
		pos = str.find(substr, pos);
		if (pos == string::npos) break;
		pos += substr.length();
		res++;
	}

	return res;
}

string tolower(string word) {
	string res;
	for (int sym = 0; sym < (word).length(); sym++) {
		res += tolower(word[sym]);
	}

	return res;
}

bool entriesChars(char ch, vector<int> chars) {
	for (int chNum : chars) {
		if (ch == chNum) return true;
	}
	return false;
}

void clearPuncts(string& word) {
	while (word.length() > 1) {
		if (65 >= word[0] || word[0] >= 122) {
			word = word.substr(1, word.length() - 1);
		}

		if ((65 >= word[word.length() - 1] || word[word.length() - 1] >= 122) && word.length() != 1) {
			word = word.substr(0, word.length() - 1);
		}
		else break;
	}
}

void task1() {
	fstream file("..\\..\\files\\lab06.txt");

	if (!file.is_open()) {
		cout << "[ ERROR ] Can't open file!" << endl;
	}

	string line;
	int numbers = 0, letters = 0, puncMarks = 0, symbols = 0; //add symbols counter

	while (getline(file, line)) {
		for (int sym = 0; sym < line.length(); sym++, symbols++) {
			if (65 <= line[sym] && line[sym] <= 122) letters++;

			if (48 <= line[sym] && line[sym] <= 57) numbers++;

			if (entriesChars(line[sym], { 33, 34, 40, 41, 44, 45, 46, 58, 59, 63 })) puncMarks++;
		}
		cout << line << endl;
	}

	cout << endl << "Numbers: " << numbers << endl;
	cout << endl << "Letters: " << letters << endl;
	cout << endl << "Punctuation marks: " << puncMarks << endl;
	cout << endl << "Symbols: " << symbols << endl;
}

void task2() {
	fstream file("..\\..\\files\\lab06.txt");

	if (!file.is_open()) {
		cout << "[ ERROR ] Can't open file!" << endl;
	}

	string line;
	int spaces = 0, numbers = 0, dashes = 0, sentences = 0;

	while (getline(file, line)) {
		for (int sym = 0; sym < line.length(); sym++) {
			if (line[sym] == 32) spaces++;

			if (48 <= line[sym] && line[sym] <= 57) numbers++;
		}
		spaces++;
		sentences += entries(line, "? ");
		sentences += entries(line, "! ");
		sentences += entries(line, ". ");

		dashes += entries(line, " - ");

		if (line[line.length() - 1] == '!' || line[line.length() - 1] == '?' || line[line.length() - 1] == '.') sentences++;

		cout << line << endl;
	}

	spaces -= numbers;
	spaces -= dashes;

	cout << endl << "Words: " << spaces << endl;
	cout << endl << "Sentences: " << sentences << endl;
}

void task3() {
	fstream file("..\\..\\files\\lab06.txt");

	if (!file.is_open()) {
		cout << "[ ERROR ] Can't open file!" << endl;
	}

	string word;
	int spaces = 0, consonants = 0, vowels = 0;

	while (file >> word) {
		cout << word << " ";

		clearPuncts(word);

		if (word.length() == 1 && (65 >= word[0] || word[0] >= 122)) continue;


		if (entriesChars(tolower(word[0]), { 97, 101, 105, 111, 117, 121 }) &&
			entriesChars(tolower(word[word.length() - 1]), { 97, 101, 105, 111, 117, 121 })) vowels++;

		if (!entriesChars(tolower(word[0]), { 97, 101, 105, 111, 117, 121 }) &&
			!entriesChars(tolower(word[word.length() - 1]), { 97, 101, 105, 111, 117, 121 })) consonants++;

	}


	cout << endl << endl << "Words (start and end at [ consonant ]): " << consonants << endl;
	cout << endl << "Words (start and end at [ vowel ]): " << vowels << endl;
}

void task4() {
	fstream file("..\\..\\files\\lab06.txt");

	if (!file.is_open()) {
		cout << "[ ERROR ] Can't open file!" << endl;
	}

	string word;
	int res = 0;
	bool doubleLettered = false;

	while (file >> word) {
		doubleLettered = false;

		clearPuncts(word);

		if (word.length() == 1) continue;

		for (int sym = 1; sym < word.length(); sym++) {
			if (tolower(word[sym - 1]) == tolower(word[sym]) && !doubleLettered) {
				res++;
				doubleLettered = true;
			}
		}

		if(doubleLettered) cout << word << " ";
	}

	cout << endl << endl << "Words with double-letters: " << res << endl;
	file.close();
}

void task5() {
	fstream file("..\\..\\files\\lab06.txt");

	if (!file.is_open()) {
		cout << "[ ERROR ] Can't open file!" << endl;
	}

	string word, longest = "NULL", shortest = "NULL";
	const int THE_LONGEST_WORD = 26;
	int max = 0, min = THE_LONGEST_WORD + 1;
	vector<string> words;

	while (file >> word) {

		clearPuncts(word);

		if (word.length() == 1 && (65 >= word[0] || word[0] >= 122)) continue;

		if (word.length() > max) {
			longest = word;
			max = word.length();
		}
		if (word.length() < min) {
			shortest = word;
			min = word.length();
		}

		cout << word << " ";
	}

	cout << endl << endl << "The longest word: [ " << longest << " ] with length of [ " << max << " ]" << endl;
	cout << endl << "The shortest word: [ " << shortest << " ] with length of [ " << min << " ]" << endl;
	file.close();
}

void task6() {
	fstream file("..\\..\\files\\lab06.txt");

	if (!file.is_open()) {
		cout << "[ ERROR ] Can't open file!" << endl;
	}

	string word, searchWord;
	cout << "Enter word to search: ";
	cin >> searchWord;
	bool found = false;;

	while (file >> word) {

		clearPuncts(word);

		if (word.length() == 1 && (65 >= word[0] || word[0] >= 122)) continue;

		

		if (tolower(word) == tolower(searchWord)) {
			cout << "Found!" << endl;
			found = true;
			break;
		}
	}

	if (!found) cout << "NOT found!" << endl;
	file.close();
}

void task7() {
	fstream file("..\\..\\files\\lab06.txt");

	if (!file.is_open()) {
		cout << "[ ERROR ] Can't open file!" << endl;
	}

	char sym;
	int pos = 0;

	for (; file;) {
		file.seekp(pos);
		file.get(sym);
		if (file.eof()) break;
		cout << sym << endl;

		if (entriesChars(tolower(sym), { 97, 101, 105, 111, 117, 121 })) {
			file.seekp(pos);
			file.write("*", 1);
		}


		pos++;
	}

	file.close();
}

void task8() {
	fstream file("..\\..\\files\\lab06.txt");

	if (!file.is_open()) {
		cout << "[ ERROR ] Can't open file!" << endl;
	}

	char sym;
	string res;
	int pos = 0;

	while (file) {
		file.seekp(pos);
		file.get(sym);

		if (!entriesChars(tolower(sym), { 97, 101, 105, 111, 117, 121 })) {
			res += sym;
		}

		if (sym == '\n') pos++;
		pos++;
	}

	cout << res << endl;
	file.close();

	file.open("..\\..\\files\\lab06.txt", ios_base::out | ios_base::trunc);
	if (!file.is_open()) {
		cout << "[ ERROR ] Can't open file!" << endl;
	}
	file << res;
}

void task9() {
	fstream file("..\\..\\files\\lab06.txt");

	if (!file.is_open()) {
		cout << "[ ERROR ] Can't open file!" << endl;
	}

	char sym;
	string res;
	int pos = 0;

	while (file) {
		file.seekp(pos);
		file.get(sym);


		if (tolower(sym) == 'k') {
			res += "math";
		} else {
			res += sym;
		}

		if (sym == 10) pos++;
		pos++;
	}

	cout << res << endl;
	file.close();

	file.open("..\\..\\files\\lab06.txt", ios_base::out | ios_base::trunc);
	if (!file.is_open()) {
		cout << "[ ERROR ] Can't open file!" << endl;
	}
	file << res;
}