#include <vector>
#include <iostream>
#include <string>
#include <cctype>
#include <cstdlib>

using namespace std;

int max(int a, int b) {
	return (a >= b) ? a : b;
}

int min(int a, int b) {
	return (a <= b) ? a : b;
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

void writeArrNum(vector<int> num) {
	for (int i = 0, j = num.size(); i < num.size(); i++, j--) {
		if ((j - 1) % 3 == 0) {
			cout << num[i] << " ";
		}
		else
			cout << num[i];
	}

	cout << endl;
}

class Number {
public:
	vector<int> digits;
	int dotPos;
	bool isPositive;

	void normalize() {
		for (int i = digits.size() - 1, j = 0; digits[i] == 0 && dotPos > 0; i--) {
			digits.pop_back();
			dotPos--;
		}

		reverse(digits.begin(), digits.end());

		for (int i = digits.size() - 1; digits[i] == 0 && i > dotPos; i--) {
			digits.pop_back();
		}

		reverse(digits.begin(), digits.end());
	}

	Number() {
		string num;
		cout << "Введите число: ";
		cin >> num;

		if (num[0] == '-') {
			isPositive = false;
			num.replace(0, 1, "");
		}
		else {
			isPositive = true;
		}

		int i = 0;
		for (bool dot = false; i < num.length(); i++) {
			if (num[i] == '.') {
				dotPos = i;
				if (dot) break;
				dot = true;
				continue;
			}
			digits.push_back((int)num[i] - '0');
		}

		if (!dotPos) {
			dotPos = 0;
		}
		else {
			dotPos = digits.size() - dotPos;
		}
	}

	Number(bool isPositive, vector<int> digits, int dotPos) {
		this->isPositive = isPositive;
		this->digits = digits;
		this->dotPos = dotPos;
	}


	void out() {

		normalize();

		if (!isPositive) {
			cout << "- ";
		}

		for (int i = 0, j = digits.size() - dotPos; i < (dotPos ? digits.size() - dotPos : digits.size()); i++, j--) {
			if (j % 3 == 0 && i != 0) {
				cout << " ";
			}
			cout << digits[i];
		}

		if (dotPos != 0) {
			cout << ".";

			for (int i = digits.size() - dotPos, j = 1; i < digits.size(); i++, j++) {
				if ((j - 1) % 3 == 0 && i != digits.size() - dotPos) {
					cout << " ";
				}
				cout << digits[i];
			}
		}
	}
};

Number operator + (Number a, Number b);
Number operator - (Number a, Number b);
bool operator >= (Number a, Number b);


vector<int> equateDigits(Number a, Number b) {
	vector<int> res = a.digits;

	if (a.dotPos >= b.dotPos) {
		return res;
	}
	else {
		int diff = abs(a.dotPos - b.dotPos);

		for (int i = diff; i > 0; i--) {
			res.push_back(0);
		}
		return res;
	}
}

Number operator + (Number a, Number b) {
	vector<int> digitsA = equateDigits(a, b);
	vector<int> digitsB = equateDigits(b, a);

	bool isPositive;
	vector<int> digits;
	int dotPos;

	writeArrNum(digitsA);
	writeArrNum(digitsB);
	cout << endl;

	if (a.isPositive && b.isPositive) {
		digits = numsPlus(digitsA, digitsB);
		isPositive = true;
		dotPos = max(a.dotPos, b.dotPos);
	}
	else if (!a.isPositive && !b.isPositive) {
		digits = numsPlus(digitsA, digitsB);
		isPositive = false;
		dotPos = max(a.dotPos, b.dotPos);
	}
	else if (a.isPositive && !b.isPositive) {
		return a - b;
	}
	else if (!a.isPositive && b.isPositive) {
		return b - a;
	}

	//cout << (int)digitsA.size() << " " << (int)digitsB.size() << endl;
	//vector<int> digits = {(int)digitsA.size(), (int)digitsB.size() };

	return Number(isPositive, digits, dotPos);
}

Number operator - (Number a, Number b) {
	vector<int> digitsA = equateDigits(a, b);
	vector<int> digitsB = equateDigits(b, a);

	bool isPositive;
	vector<int> digits;
	int dotPos;

	if (a.isPositive && b.isPositive) {
		if (a >= b) {
			digits = numsMinus(digitsA, digitsB);
			isPositive = true;
			dotPos = max(a.dotPos, b.dotPos);
		}
		else {
			digits = numsMinus(digitsB, digitsA);
			isPositive = false;
			dotPos = max(a.dotPos, b.dotPos);
		}
	}
	else if (!a.isPositive && !b.isPositive) {
		Number a1(true, a.digits, a.dotPos);
		Number b1(true, b.digits, b.dotPos);

		return b1 - a1;
	}
	else if (!a.isPositive && b.isPositive) {
		Number a1(true, a.digits, a.dotPos);
		Number b1(true, b.digits, b.dotPos);
		Number c = a1 + b1;
		c.isPositive = false;
		return c;
	}
	else if (a.isPositive && !b.isPositive) {
		Number a1(true, a.digits, a.dotPos);
		Number b1(true, b.digits, b.dotPos);
		return a1 + b1;
	}

	return Number(isPositive, digits, dotPos);
}

bool operator >= (Number a, Number b) {
	if (a.isPositive != b.isPositive) {
		return (a.isPositive ? true : false);
	}
	else {
		bool res;

		if (a.digits.size() - a.dotPos > b.digits.size() - b.dotPos) {
			res = true;
		}
		else if (a.digits.size() - a.dotPos < b.digits.size() - b.dotPos) {
			res = false;
		}
		else {
			vector<int> digitsA = equateDigits(a, b);
			vector<int> digitsB = equateDigits(b, a);

			for (int i = 0; i < digitsA.size(); i++) {
				if (digitsA[i] > digitsB[i]) {
					res = true;
					break;
				}
				if (digitsA[i] < digitsB[i]) {
					res = false;
					break;
				}
				if (i == digitsA.size() - 1 && digitsA[i] == digitsB[i]) res = true;
			}

		}

		if (a.isPositive) {
			return res;
		}
		else {
			return !res;
		}
	}
}

Number operator * (Number a, Number b) {
	//vector<int> digitsA = equateDigits(a, b);
	//vector<int> digitsB = equateDigits(b, a);

	vector<int> digitsA = a.digits;
	vector<int> digitsB = b.digits;

	bool isPositive;
	vector<int> digits;
	int dotPos;

	if (a.isPositive == b.isPositive) {
		digits = numsMultiply(digitsA, digitsB);
		dotPos = a.dotPos + b.dotPos;
		isPositive = true;

		if (dotPos >= digits.size()) {
			cout << "Adding zeros" << endl;
			reverse(digits.begin(), digits.end());

			for (int i = dotPos - digits.size(); i > 0; i--) {
				digits.push_back(0);
			}

			reverse(digits.begin(), digits.end());
		}
		
	}
	else {
		Number a1(true, a.digits, a.dotPos);
		Number b1(true, b.digits, b.dotPos);
		Number c = a1 * b1;
		c.isPositive = false;
		return c;
	}

	return Number(isPositive, digits, dotPos);
}

Number operator / (Number a, Number b) {
	Number a1 = a, b1 = b;

	int maxPos = max(a.dotPos, b.dotPos);

	for (int i = maxPos; i > 0; i--) {
		a1 = a1 * Number(true, vector<int> {1,0}, 0);
		b1 = b1 * Number(true, vector<int> {1,0}, 0);
	}

	a1.normalize();
	b1.normalize();

	vector<int> digitsA = a1.digits;
	vector<int> digitsB = b1.digits;

	writeArrNum(digitsA);
	writeArrNum(digitsB);

	bool isPositive;
	vector<int> digits;
	int dotPos = 0;

	if (a1.isPositive != b1.isPositive) {
		isPositive = false;
	}
	else {
		isPositive = true;
	}

	if (b1.digits.size() == 1 && b1.digits[0] == 1) {
		return Number(isPositive, a1.digits, a1.dotPos);
	}

	if (a1.digits.size() < b1.digits.size()) {
		digits.push_back(0);
		//cout << "Added zero" << endl;
	}

	int accuracy = 0;
	cout << "Введите точность деления: ";
	cin >> accuracy;

	vector<int> divided;

	for (int digit = 0; digit < (digitsA.size() + accuracy); ) {
		int fractionDigit = 0;

		//cout << "outer " << digit << endl;

		for (int i = 0; isGreater(digitsB, divided) && digit < (digitsA.size() + accuracy); digit++, i++) {
			
			//cout << "i: " << i << "  digit: " << digit << endl;
			
			/*
			if (digit >= digitsA.size()) {
				divided.push_back(0);
				if (!isGreaterEqual(divided, digitsB)) {
					digits.push_back(0);
				}
				dotPos++;
				i--;
				//digitsA.push_back(0);
				//cout << "added zero" << endl;
				continue;
			}
			divided.push_back(digitsA[digit]);
			if (i >= 1 && !digits.empty()) {
				digits.push_back(0);
			}
			*/
			
			if (divided.empty() && digitsA[digit] == 0) {
				digits.push_back(0);
				//cout << "Moving 0: added 0" << endl;
				i--;
			}
			else {
				divided.push_back(digitsA[digit]);

				if (!digits.empty() && isGreaterEqual(divided, digitsB) && i > 0) {
					digits.push_back(0);
					//cout << "This digit isn't enought:  added 0" << endl;
				}
			}
		}


		if (digit == (digitsA.size() + accuracy) && !isGreater(divided, digitsB)) {
			int remainderDigits = digitsA.size() - numsMultiply(digits, digitsB).size();
			for (int i = 0; i < remainderDigits; i++) {
				digits.push_back(0);
				divided.pop_back();
			}
			//cout << "Break" << endl;
			break;
		}

		while (isGreaterEqual(divided, digitsB)) {

			//cout << "BEFORE inner divider: ";
			//writeArrNum(divided);

			divided = numsMinus(divided, digitsB);
			fractionDigit++;

			//cout << "AFTER inner divider: ";
			//writeArrNum(divided);
		}


		//cout << "fraction digit: " << fractionDigit << endl;


		digits.push_back(fractionDigit);
		if (digit >= digitsA.size() && divided.empty()) break;
		//cout << "DIGITS: ";
		//writeArrNum(digits);

	}

	//cout << "dotPos: " << dotPos << endl;
	//cout << "DIGITS: ";
	//writeArrNum(digits);

	return Number(isPositive, digits, dotPos);
}

int main()
{
	setlocale(LC_ALL, "Russian");

	Number a, b;
	/*
	a.out();
	cout << endl;
	b.out();
	cout << endl;
	*/

	Number c = a / b;

	c.out();
    //cout << endl << c.dotPos << endl;
	return 0;
}

