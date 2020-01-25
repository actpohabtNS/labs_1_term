#include <iostream>
#include <vector>

using namespace std;

bool includes(vector<int> vect, int elem) {
	for (int i = 0; i < vect.size(); i++) {
		if (vect[i] == elem) {
			return true;
		}
	}
	return false;
}

vector<int> numToVect(int num) {
	vector<int> vect;

	for (int i = num; i != 0; i /= 10) {
		vect.push_back(i % 10);
	}
	reverse(vect.begin(), vect.end());

	return vect;
}

bool isPrime(int num) {
	for (int i = 2; i < num; i++) {
		int j = num - 1;
		for (j; j > 1; j--) {
			if (num % i == 0) return false;
		}
		if (i == 1) cout << num << " ";
		return true;
	}
}

//TASK 1
void digitsAreUnique() {
	int num = 0;
	cout << "Enter num: ";
	cin >> num;

	cout << "-----------------------" << endl;

	vector<int> digits = numToVect(num);

	for (int i = 0; i < digits.size(); i++) {


		for (int j = i + 1; j < digits.size(); j++) {
			if (digits[i] == digits[j]) {
				cout << "Num is NOT unique" << endl;
				return;
			}
		}
	}
	cout << "Num IS unique" << endl;
	return;
}

//TASK 2
void digitsAreEven() {
	int num = 0;
	cout << "Enter num: ";
	cin >> num;

	cout << "-----------------------" << endl;

	vector<int> digits = numToVect(num);

	for (int i = 0; i < digits.size(); i++) {
		if (digits[i] % 2 == 1) {
			cout << "Digits are NOT even" << endl;
			return;
		}
	}
	cout << "Digits ARE even" << endl;
	return;
}

//TASK 3
void sumIsPrime() {
	int num = 0, sum = 0;
	cout << "Enter num: ";
	cin >> num;

	cout << "-----------------------" << endl;

	for (int i = num; i != 0; i /= 10) {
		sum += i % 10;
	}

	if (isPrime(sum)) {
		cout << "Sum IS prime" << endl;
	}
	else {
		cout << "Sum is NOT prime" << endl;
	}
}

//TASK 4
void oddEvenDigits() {
	int num = 0;
	cout << "Enter num: ";
	cin >> num;

	cout << "-----------------------" << endl;

	vector<int> digits = numToVect(num);

	for (int i = 1; i <= digits.size(); i++) {
		if (i % 2 == 1) {
			if (digits[i - 1] % 2 != 0) {
				cout << "false" << endl;
				return;
			}
		}
		else {
			if (digits[i - 1] % 2 != 1) {
				cout << "false" << endl;
				return;
			}
		}
	}

	cout << "true" << endl;
}

//TASK 5
void toNumSystem() {
	int num = 0, numSys = 10;
	vector<int> digits;

	cout << "Enter num: ";
	cin >> num;
	cout << "Enter number system: ";
	cin >> numSys;

	cout << "-----------------------" << endl;

	while (num != 0) {
		digits.push_back(num % numSys);
		num /= numSys;
	}

	reverse(digits.begin(), digits.end());

	cout << "Here is your num: ";

	for (int i = 0; i < digits.size(); i++) {
		cout << digits[i];
	}
}

int main()
{
	int task = -1, from = 1, to = 5;

	cout << "If you want to stop, enter 0" << endl;


	while (task != 0) {
		cout << endl;
		cout << "Enter task num FROM " << from << " TO " << to << ": ";
		cin >> task;

		while (task != 0 && task < from || task > to) {
			cout << "WRONG!" << endl;
			cout << "Enter task num FROM " << from << " TO " << to << ": ";
			cin >> task;
		}

		switch (task) {
		case 1:
			cout << "Task: digitsAreUnique" << endl;
			digitsAreUnique();
			break;
		case 2:
			cout << "Task: digitsAreEven" << endl;
			digitsAreEven();
			break;
		case 3:
			cout << "Task: sumIsPrime" << endl;
			sumIsPrime();
			break;
		case 4:
			cout << "Task: oddEvenDigits" << endl;
			oddEvenDigits();
			break;
		case 5:
			cout << "Task: toNumSystem" << endl;
			toNumSystem();
			break;
		}

	}

	return 0;
}
