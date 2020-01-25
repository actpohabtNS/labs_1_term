#include <vector>
#include <iostream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <map>

using namespace std;

int max(int a, int b) {
	return (a >= b) ? a : b;
}

int min(int a, int b) {
	return (a <= b) ? a : b;
}

string clearSym(string str, char sym) {
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == sym)
		{
			str.erase(i, 1);
			i--;
		}
	}
	return str;
}

void writeArrNum(vector<int> num) {
	for (int i = 0, j = num.size(); i < num.size(); i++, j--) {
		cout << num[i];
	}
}

void writeDelimeters(map <int, int> map) {
	for (auto& item : map) {
		cout << item.first << " ^ " << item.second << " * ";
	}
}

vector<int> numToVect(int num) {
	vector<int> numVect;
	while (num > 0) {
		numVect.push_back(num % 10);
		num /= 10;
	}

	reverse(numVect.begin(), numVect.end());

	return numVect;
}

bool isGreater(vector<int> num1, vector<int> num2) {
	if (num1.size() > num2.size()) {
		return true;
	}
	else if (num1.size() < num2.size()) {
		return false;
	}
	else {
		for (int i = 0; i < num1.size(); i++) {
			if (num1[i] > num2[i]) return true;
			if (num1[i] < num2[i]) return false;
		}
		return false;
	}
}

bool isGreaterEqual(vector<int> num1, vector<int> num2) {
	if (num1.size() > num2.size()) {
		return true;
	}
	else if (num1.size() < num2.size()) {
		return false;
	}
	else {
		for (int i = 0; i < num1.size(); i++) {
			if (num1[i] > num2[i]) return true;
			if (num1[i] < num2[i]) return false;
		}
		return true;
	}
}

bool isEqual(vector<int> num1, vector<int> num2) {
	if (num1.size() != num2.size()) return false;
	else {
		for (int i = 0; i < num1.size(); i++) {
			if (num1[i] != num2[i]) return false;
		}
		return true;
	}
}

vector<int> numsPlus(vector<int> num1, vector<int> num2) {
	vector<int> res;

	reverse(num1.begin(), num1.end());
	reverse(num2.begin(), num2.end());

	int digitUp = 0;

	for (int i = 0; (i < num1.size() || i < num2.size()); i++) {
		//cout << "i: " << i << endl;

		if (i >= num1.size()) {
			//cout << "adding num2: " << num2[i] + digitUp << endl;
			res.push_back((num2[i] + digitUp) % 10);
			digitUp = (num2[i] + digitUp) / 10;
			continue;
		}
		if (i >= num2.size()) {
			res.push_back((num1[i] + digitUp) % 10);
			digitUp = (num1[i] + digitUp) / 10;
			continue;
		}

		if (i > num1.size() && i > num2.size()) {
			res.push_back(digitUp);
			//cout << "ended" << endl;
			break;
		}

		res.push_back((num1[i] + num2[i] + digitUp) % 10);
		if (num1[i] + num2[i] + digitUp >= 10) {
			//cout << "adding: " << (num1[i] + num2[i] + digitUp) % 10 << endl;
			digitUp = (num1[i] + num2[i] + digitUp) / 10;
		}
		else {
			//cout << "adding: " << num1[i] + num2[i] + digitUp << endl;
			//res.push_back(num1[i] + num2[i] + digitUp);
			digitUp = 0;
		}

		//cout << "digitUp " << digitUp << endl;
		//cout << endl;
	}

	if (digitUp > 0) {
		res.push_back(digitUp);
	}

	reverse(res.begin(), res.end());

	return res;
}

vector<int> numsMinus(vector<int> num1, vector<int> num2) {
	vector<int> res, testedRes;

	reverse(num1.begin(), num1.end());
	reverse(num2.begin(), num2.end());

	int digitDown = 0;

	for (int i = 0; (i < num1.size() || i < num2.size()); i++) {
		if (i >= num1.size()) {
			//cout << "adding num2: " << num2[i] + digitUp << endl;
			if (num2[i] - digitDown == 0) {
				//cout << "adding: " << (num2[i] + digitUp) % 10 << endl;
				//res.push_back(0);
				digitDown = 0;
			}
			else {
				res.push_back(num2[i] - digitDown);
				digitDown = 0;
			}
			continue;
		}
		if (i >= num2.size()) {
			if (num1[i] + digitDown == 0) {
				//cout << "adding: " << (num1[i] + digitUp) % 10 << endl;
				//res.push_back(0);
				digitDown = 0;
			}
			else {
				res.push_back(num1[i] - digitDown);
				digitDown = 0;
			}
			continue;
		}


		if (num1[i] - num2[i] - digitDown < 0) {
			//cout << "adding: " << (num1[i] + num2[i] + digitUp) % 10 << endl;
			res.push_back(10 + num1[i] - num2[i] - digitDown);
			digitDown = 1;
		}
		else {
			//cout << "adding: " << num1[i] + num2[i] + digitUp << endl;
			res.push_back(num1[i] - num2[i] - digitDown);
			digitDown = 0;
		}

		//cout << "digitUp " << digitUp << endl;
		//cout << endl;
	}

	if (digitDown > 0) {
		res.push_back(digitDown);
	}

	reverse(res.begin(), res.end());

	if (res[0] == 0) {
		int k = 0;
		for (; k < res.size() && res[k] == 0; k++);
		res.erase(res.begin(), res.begin() + k);
	}

	//reverse(testedRes.begin(), testedRes.end());

	return res;
}

vector<int> numsMultiply(vector<int> num1, vector<int> num2) {
	vector<vector<int>> partSums;

	reverse(num1.begin(), num1.end());
	reverse(num2.begin(), num2.end());


	for (int i = 0; i < num2.size(); i++) {
		vector<int> partSum;
		int digitUp = 0;

		for (int upperDigit = i; upperDigit > 0; upperDigit--) {
			partSum.push_back(0);
		}

		for (int j = 0; j < num1.size(); j++) {

			partSum.push_back((num1[j] * num2[i] + digitUp) % 10);

			digitUp = (num1[j] * num2[i] + digitUp) / 10;

			if (j == num1.size() - 1 && digitUp != 0) {
				partSum.push_back(digitUp);
				digitUp = 0;
			}
		}
		reverse(partSum.begin(), partSum.end());
		partSums.push_back(partSum);
	}

	vector<int> res = partSums[0];
	for (int i = 1; i < partSums.size(); i++) {
		res = numsPlus(res, partSums[i]);
	}
	return res;
}

vector<int> numsDivide(vector<int> num1, vector<int> num2) {
	vector<int> divided, res;

	for (int digit = 0; digit < num1.size(); ) {
		int fractionDigit = 0;

		//cout << "outer " << digit << endl;

		for (int i = 0; isGreater(num2, divided) && digit < num1.size(); digit++, i++) {

			//cout << "innerPlusser: " << num1[digit] << "  i: " << i << endl;

			if (divided.empty() && num1[digit] == 0) {
				res.push_back(0);
				//cout << "Moving 0: added 0" << endl;
				i--;
			}
			else {
				divided.push_back(num1[digit]);

				if (!res.empty() && isGreaterEqual(divided, num2) && i > 0) {
					res.push_back(0);
					//cout << "This digit isn't enought:  added 0" << endl;
				}
			}
			//if (!divided.empty() && i >= 1 && !res.empty()) res.push_back(0);
		}

		if (digit == num1.size() && !isGreaterEqual(divided, num2)) {
			int remainderDigits = num1.size() - numsMultiply(res, num2).size();
			for (int i = 0; i < remainderDigits; i++) {
				res.push_back(0);
				divided.pop_back();
			}
			//cout << "break" << endl;
			break;
		}

		while (isGreaterEqual(divided, num2)) {
			//cout << "BEFORE inner divider: ";
			//writeArrNum(divided);

			divided = numsMinus(divided, num2);
			fractionDigit++;

			//cout << "AFTER inner divider: ";
			//writeArrNum(divided);
		}

		//cout << "Divided: ";
		//writeArrNum(divided);
		//cout << "fraction digit: " << fractionDigit << endl;
		res.push_back(fractionDigit);

	}

	return res;
}

vector<int> getRemainder(vector<int> num1, vector<int> num2) {
	vector<int> divided, res;

	for (int digit = 0; digit < num1.size(); ) {
		int fractionDigit = 0;

		//cout << "outer " << digit << endl;

		for (int i = 0; isGreater(num2, divided) && digit < num1.size(); digit++, i++) {

			//cout << "innerPlusser: " << num1[digit] << "  i: " << i << endl;

			divided.push_back(num1[digit]);
			if (i >= 1 && !res.empty()) res.push_back(0);
		}

		if (digit == num1.size() && !isGreaterEqual(divided, num2)) {
			int remainderDigits = num1.size() - numsMultiply(res, num2).size();
			for (int i = 0; i < remainderDigits; i++) {
				res.push_back(0);
			}
			//cout << "break" << endl;
			break;
		}

		while (isGreaterEqual(divided, num2)) {
			//cout << "BEFORE inner divider: ";
			//writeArrNum(divided);

			divided = numsMinus(divided, num2);
			fractionDigit++;

			//cout << "AFTER inner divider: ";
			//writeArrNum(divided);
		}

		//cout << "fraction digit: " << fractionDigit << endl;
		res.push_back(fractionDigit);

	}

	return divided;
}

map <int, int> getDelimiters(vector<int> num) {
	vector<int> digits = num;
	map <int, int> delimiters;

	while (!isEqual(digits, vector<int> {1})) {
		//cout << "while: ";
		//writeArrNum(digits);



		if (getRemainder(digits, numToVect(2)).empty() || isEqual(getRemainder(digits, numToVect(2)), vector<int> {0})) {

			if (delimiters.find(2) == delimiters.end()) {
				delimiters.insert(pair<int, int>(2, 1));
			}
			else {
				delimiters[2]++;
			}

			//cout << "Dividing" << endl;
			digits = numsDivide(digits, vector<int> {2});

		}
		else {


			for (int delimiter = 3; isGreaterEqual(digits, numToVect(delimiter)); delimiter+=2) {
				//cout << "for ";

				if (!getRemainder(digits, numToVect(delimiter)).empty()) {
					//cout << "continue: " << delimiter << endl;
				}
				else {
					//cout << "delimeter with: " << delimiter << endl;
					if (delimiters.find(delimiter) == delimiters.end()) {
						delimiters.insert(pair<int, int>(delimiter, 1));
					}
					else {
						delimiters[delimiter]++;
					}

					digits = numsDivide(digits, numToVect(delimiter));
					//cout << "Break: " << delimiter << endl;
					break;

				}

			}

		}
	}

	return delimiters;
}

map <int, int> getCommonDelimeters(map <int, int> del1, map <int, int> del2) {
	map <int, int> res;

	for (auto& item1 : del1) {
		for (auto& item2 : del2) {
			if (item1.first == item2.first) res.insert(pair<int, int>(item1.first, min(item1.second, item2.second)));
		}
	}

	return res;
}

void reduceNums(vector<int> &num1, vector<int> &num2) {
	map <int, int> dels = getCommonDelimeters(getDelimiters(num1), getDelimiters(num2));

	//cout << "Common dividers: " << endl;
	//writeDelimeters(dels);

	for (auto& del : dels) {
		for (int i = 0; i < del.second; i++) {
			num1 = numsDivide(num1, numToVect(del.first));
			num2 = numsDivide(num2, numToVect(del.first));
		}
	}

	
	//cout << endl;
	//cout << "Reduced nums: " << endl;
	//writeArrNum(num1);
	//writeArrNum(num2);
	
}


class Fraction {
public:
	vector<int> nominator, denominator;
	bool isPositive = true;

	vector<int> numIn(string num) {
		vector<int> res;

		if (num[0] == '-') {
			isPositive = !isPositive;
			num.replace(0, 1, "");
		}

		for (int i = 0; i < num.length(); i++) {
			res.push_back((int)num[i] - '0');
		}

		return res;
	}

	void reduce() {
		reduceNums(nominator, denominator);
	}

	Fraction() {
		nominator = vector<int> {1};
		denominator = vector<int>{ 1 };
	}

	Fraction(string fract) {
		int delPos = fract.find("/");
		nominator = this->numIn(fract.substr(0, delPos));
		denominator = this->numIn(fract.substr(delPos+1, fract.length()));

		if (isEqual(denominator, vector<int> {0})) {
			cout << "Dividing by 0" << endl;

			system("pause");
		}
	}

	Fraction(bool isPositive, vector<int> nominator, vector<int> denominator) {
		this->isPositive = isPositive;
		this->nominator = nominator;
		this->denominator = denominator;

		if (isEqual(denominator, vector<int> {0})) {
			cout << "Dividing by 0" << endl;
			system("pause");
		}
	}

	void out() {

		if (!isPositive) {
			cout << "- ";
		}

		if (isEqual(denominator, vector<int> {1})) {
			for (int digit : nominator) {
				cout << digit;
			}
		}
		else {

			if (nominator > denominator) {

				writeArrNum(numsDivide(nominator, denominator));
				cout << " ";

				writeArrNum(getRemainder(nominator, denominator));
				cout << "/";

				for (int digit : denominator) {
					cout << digit;
				}
			}
			else {

				for (int digit : nominator) {
					cout << digit;
				}

				cout << "/";

				for (int digit : denominator) {
					cout << digit;
				}
			}
		}
	}
};


bool operator > (Fraction fract1, Fraction fract2) {
	return isGreater(numsMultiply(fract1.nominator, fract2.denominator), numsMultiply(fract2.nominator, fract1.denominator));
}
Fraction operator + (Fraction fract1, Fraction fract2);
Fraction operator - (Fraction fract1, Fraction fract2);


Fraction operator + (Fraction fract1, Fraction fract2) {
	bool isPositive;
	vector<int> nominator, denominator;

	if (fract1.isPositive != fract2.isPositive) {
		if (fract1.isPositive) {
			return fract1 - fract2;
		}
		else {
			return fract2 - fract1;
		}
	}
	else {
		if (fract1.isPositive) {
			isPositive = true;
		}
		else {
			isPositive = false;
		}
		nominator = numsPlus(numsMultiply(fract1.nominator, fract2.denominator), numsMultiply(fract2.nominator, fract1.denominator));
		denominator = numsMultiply(fract1.denominator, fract2.denominator);
	}

	Fraction res(isPositive, nominator, denominator);
	res.reduce();
	return res;
}

Fraction operator - (Fraction fract1, Fraction fract2) {
	bool isPositive;
	vector<int> nominator, denominator;

	if (fract1.isPositive && fract2.isPositive) {
		if (fract1 > fract2) {
			isPositive = true;
			nominator = numsMinus(numsMultiply(fract1.nominator, fract2.denominator), numsMultiply(fract2.nominator, fract1.denominator));
			denominator = numsMultiply(fract1.denominator, fract2.denominator);
		}
		else {
			isPositive = false;
			nominator = numsMinus(numsMultiply(fract2.nominator, fract1.denominator), numsMultiply(fract1.nominator, fract2.denominator));
			denominator = numsMultiply(fract1.denominator, fract2.denominator);
		}
	}
	else if (!fract1.isPositive && !fract2.isPositive) {
		return fract2 - fract1;
	}
	else if (!fract1.isPositive && fract2.isPositive) {
		Fraction c = Fraction(true, fract1.nominator, fract1.denominator) + fract2;
		c.isPositive = false;
		return c;
	}
	else if (fract1.isPositive && !fract2.isPositive) {
		return fract1 + Fraction(true, fract2.nominator, fract2.denominator);
	}

	Fraction res(isPositive, nominator, denominator);
	res.reduce();
	return res;
}

Fraction operator * (Fraction fract1, Fraction fract2) {
	bool isPositive = fract1.isPositive == fract2.isPositive;
	vector<int> nominator, denominator;

	nominator = numsMultiply(fract1.nominator, fract2.nominator);
	denominator = numsMultiply(fract1.denominator, fract2.denominator);

	Fraction res(isPositive, nominator, denominator);
	res.reduce();
	return res;
}

Fraction operator / (Fraction fract1, Fraction fract2) {
	bool isPositive = fract1.isPositive == fract2.isPositive;
	vector<int> nominator, denominator;


	if (isEqual(fract2.nominator, vector<int> {0})) {
		cout << "Dividing by 0" << endl;
		system("pause");
	}

	nominator = numsMultiply(fract1.nominator, fract2.denominator);
	denominator = numsMultiply(fract1.denominator, fract2.nominator);

	Fraction res(isPositive, nominator, denominator);
	res.reduce();
	return res;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	string expr;
	cout << "Введите выражение: ";
	getline(cin, expr);
	expr = clearSym(expr, ' ');

	int exprPos = expr.find('/');


	if (expr[exprPos + 1] == '-') {
		exprPos++;
	}

	while (isdigit(expr[++exprPos])) {
	}


	Fraction fract1 = expr.substr(0, exprPos);
	char op = expr[exprPos++];
	Fraction fract2 = expr.substr(exprPos, expr.length());

	Fraction fract3;

	switch (op) {
	case '+': {
		fract3 = fract1 + fract2;
		break;
	}
	case '-': {
		fract3 = fract1 - fract2;
		break;
	}
	case '*': {
		fract3 = fract1 * fract2;
		break;
	}
	case '/': {
		fract3 = fract1 / fract2;
		break;
	}
	}

	fract3.reduce();
	fract3.out();

	//cout << fract1 << " and " << op << " and " << fract2 << endl;
}
