#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

const double MIN_TEMP = -89.2, MAX_TEMP = 58;

string tolower(string word) {
	string res;
	for (int sym = 0; sym < (word).length(); sym++) {
		res += tolower(word[sym]);
	}

	return res;
}

double max(double a, double b) {
	return (a > b) ? a : b;
}

double min(double a, double b) {
	return (a < b) ? a : b;
}

double min(double a, double b, double c, double d) {
	return min(min(a, b), min(c, d));
}

double max(double a, double b, double c, double d) {
	return max(max(a, b), max(c, d));
}

struct countryTemp {
	string country;
	double spring, summer, fall, winter, average;
	string topSeason;
	double getTemp(string par) {
		if (par == "summer") {
			return summer;
		}
		else if (par == "fall") return fall;
		else if (par == "winter") return winter;
		else if (par == "spring") return spring;
		else if (par == "average") return average;
		else return -666;
	}
};

void compareTemp(countryTemp& maxTemp, countryTemp& minTemp, countryTemp cT, string season) {
	if (maxTemp.getTemp(season) <= cT.getTemp(season)) {
		maxTemp = cT;
	}

	if (minTemp.getTemp(season) >= cT.getTemp(season)) {
		minTemp = cT;
	}
}

void swap(countryTemp& cT1, countryTemp& cT2) {
	countryTemp temp = cT1;
	cT1 = cT2;
	cT2 = temp;
}

vector<countryTemp> sortCoun(vector<countryTemp> cTs, string season, bool asc) {

	int i, j;
	bool swapped;
	for (i = 0; i < cTs.size(); i++)
	{
		swapped = false;
		for (j = 0; j < cTs.size() - i - 1; j++)
		{
			if ((asc && cTs[j].getTemp(season) > cTs[j + 1].getTemp(season)) || (!asc && cTs[j].getTemp(season) < cTs[j + 1].getTemp(season)))
			{
				swap(cTs[j], cTs[j + 1]);
				swapped = true;
			}
		}

		if (swapped == false)
			break;
	}

	cout << "=============== " << season << " ===============" << endl;

	cout << "[ Ascending ] temps: " << endl;
	for (countryTemp coun : cTs) {
		cout << coun.country << " (" << coun.getTemp(season) << "); " << endl;
	}
	cout << endl << endl;

	cout << "[ Descending ] temps: " << endl;
	for (int i = cTs.size()-1; i >= 0 ; i--) {
		cout << cTs[i].country << " (" << cTs[i].getTemp(season) << "); " << endl;
	}

	cout << endl << "========================================" << endl << endl;

	return cTs;
}

void findMinMax(vector<countryTemp> cTs, string season, countryTemp &minCT, countryTemp &maxCT) {

	if (cTs[cTs.size() - 1].getTemp(season) >= maxCT.summer) {
		maxCT = cTs[cTs.size() - 1];
		maxCT.topSeason = season;
	}
	if (cTs[0].getTemp(season) <= minCT.winter) {
		minCT = cTs[0];
		minCT.topSeason = season;
	}

	cout << "[ TOP ] [ " << season << " ] temp(s): " << endl;
	cout << cTs[cTs.size() - 1].country << " (" << cTs[cTs.size() - 1].getTemp(season) << "); ";

	for (int i = cTs.size() - 2; i >= 0; i--) {
		if (cTs[i].getTemp(season) == cTs[i + 1].getTemp(season)) {
			cout << cTs[i].country << " (" << cTs[i].getTemp(season) << "); ";
		}
		else break;
	}

	cout << endl << endl;
	
	cout << "[ MIN ] [ " << season << " ] temp(s): " << endl;
	cout << cTs[0].country << " (" << cTs[0].getTemp(season) << "); ";

	for (int i = 1; i < cTs.size(); i++) {
		if (cTs[i].getTemp(season) == cTs[i - 1].getTemp(season)) {
			cout << cTs[i].country << " (" << cTs[i].getTemp(season) << "); ";
		}
		else break;
	}
	cout << endl << endl;
}

void task1() {
	fstream file("..\\..\\files\\lab07.txt");

	if (!file.is_open()) cout << "[ ERROR ] File can not be opened!" << endl;

	countryTemp cT;

	const double MIN_TEMP = -89.2, MAX_TEMP = 58;

	vector<countryTemp> ascCoun;

	int readingElem = 0;

	while (!file.eof()) {
		readingElem++;
		file >> cT.country >> cT.summer >> cT.fall >> cT.winter >> cT.spring;
		cT.average = (cT.spring + cT.summer + cT.fall + cT.winter) / 4;

		ascCoun.push_back(cT);
		
	}

	vector<countryTemp> ascSumCount = sortCoun(ascCoun, "summer", true);
	vector<countryTemp> ascFallCount = sortCoun(ascCoun, "fall", true);
	vector<countryTemp> ascWinCount = sortCoun(ascCoun, "winter", true);
	vector<countryTemp> ascSprCount = sortCoun(ascCoun, "spring", true);
	vector<countryTemp> ascAveCount = sortCoun(ascCoun, "average", true);

	countryTemp maxCoun = { "NULL", MIN_TEMP, MIN_TEMP, MIN_TEMP, MIN_TEMP }, minCoun = {"NULL", MAX_TEMP, MAX_TEMP, MAX_TEMP, MAX_TEMP};

	findMinMax(ascSumCount, "summer", minCoun, maxCoun);
	findMinMax(ascFallCount, "fall", minCoun, maxCoun);
	findMinMax(ascWinCount, "winter", minCoun, maxCoun);
	findMinMax(ascSprCount, "spring", minCoun, maxCoun);
	findMinMax(ascAveCount, "average", minCoun, maxCoun);

	cout << endl << endl << "[ MAX ] temp: " << maxCoun.country << ": " << maxCoun.topSeason << " (" << maxCoun.summer << "); " << endl;

	cout << endl << endl << "[ MIN ] temp: " << minCoun.country << ": " << minCoun.topSeason << " (" << minCoun.winter << "); " << endl;
	

	file.close();
}

void task2() {
	string ans;
	bool correctAns = false, save = false;
	while (!correctAns) {
		cout << "Do you want to -=[ SAVE ]=- country: ";
		cin >> ans;
		if (tolower(ans) == "yes" || tolower(ans) == "нуы" || tolower(ans) == "да" || tolower(ans) == "так") {
			save = true;
			correctAns = true;
		}
		else if (tolower(ans) == "no" || tolower(ans) == "то" || tolower(ans) == "нет" || tolower(ans) == "ні") {
			save = false;
			correctAns = true;
		}
	}


	if (save) {
		fstream file("..\\..\\files\\lab07.txt", ios_base::app);
		if (!file.is_open()) {
			cout << endl << "[ ERROR ] File can not be opened!" << endl;
		} else cout << endl << "File WILL be edited!" << endl;
		file.close();
	}

	string country;
	double summer, fall, winter, spring, amplitude;

	cout << endl << "Enter country's -=[ name ]=-: ";
	cin >> country;
	cout << endl << "Enter country's -=[ temperatures ]=- (summer, fall, winter, spring): ";
	cin >> summer >> fall >> winter >> spring;
	cout << endl << "Enter -=[ amplitude ]=- to check: ";
	cin >> amplitude;

	double average = (summer + fall + winter + spring) / 4,
		minT = min(summer, fall, winter, spring),
		maxT = max(summer, fall, winter, spring),
		amplitudeT = maxT - minT;

	cout << endl << "Max temp: " << maxT << "; Min temp: " << minT << "; Average temp: " << average << "; " << endl;

	if (amplitude > amplitudeT) cout << endl << "Your amplitude is [ highter ]! (" << amplitude << " > " << amplitudeT << ")" << endl;
	else if (amplitude < amplitudeT) cout << endl << "Your amplitude is [ lower ]! (" << amplitude << " < " << amplitudeT << ")" << endl;
	else cout << endl << "Your amplitude is [ equal ]! (" << amplitude << " = " << amplitudeT << ")" << endl;

	cout << "Country's amplitude: " << amplitudeT << endl;

	if (save) {
		fstream file("..\\..\\files\\lab07.txt", ios_base::app);
		if (!file.is_open()) cout << endl << "[ ERROR ] File can not be opened!" << endl;
		else {
			file << '\n' << country << " " << summer << " " << fall << " " << winter << " " << spring;
			cout << "Saved!" << endl;
		}
		file.close();
	}

}