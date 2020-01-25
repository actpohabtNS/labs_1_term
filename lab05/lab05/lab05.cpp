#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

void task1() {
	float r, x, y;
	cout << "Enter radius, x, y: ";
	cin >> r >> x >> y;

	cout << (2*r >= sqrt(x * x + y * y) ? "yes" : "no");
}

void task2()
{
	float a, b, x, y, z;
	bool possibility = false;
	cout << "Enter Hole (a, b) and Object (x, y, z): ";
	cin >> a >> b >> x >> y >> z;

	if (a >= x && b >= y) {
		cout << "Obj CAN be pushed into the hole by coords: " << x << " " << y;
		possibility = true;
	}
	if (a >= x && b >= z) {
		cout << "Obj CAN be pushed into the hole by coords: " << x << " " << z;
		possibility = true;
	}
	if (a >= y && b >= z) {
		cout << "Obj CAN be pushed into the hole by coords: " << y << " " << z;
		possibility = true;
	}

	if (!possibility)
	{
		cout << "Obj can NOT be pushed into the hole!";
	}
}

void task3() {
	int n, x, y;
	cout << "Enter NUMBER of points, Key point (x,y): ";
	 
}

void task4() {
	int r;
	cout << "Enter Radius: ";
	cin >> r;
	cin.get();

	vector<int> ratio;
	string rat;
	cout << "Enter ratio numbers: ";
	getline(cin, rat);

	string temp = "";
	for (int i = 0; i < rat.length(); i++) {
		if (rat[i] == ' ') {
			ratio.push_back(stoi(temp));
			temp = "";
		} else {
			temp += rat[i];
			if (i == rat.length() - 1) ratio.push_back(stoi(temp));
		}
	}

	int sum = 0;
	for (int num : ratio) {
		sum += num;
	}

	const double pi = M_PI;
	double oneSquare = M_PI * r * r / sum;

	for (int num = 0; num < ratio.size(); num++) {
		double value = ratio[num] * oneSquare;
		cout << fixed;
		cout << setprecision(10);
		cout << ratio[num] << " part(s): " << value << endl;
	}
}

void task5() {
	int size_x, size_y, ferzinsNum = 3;

	cout << "Enter Size (X, Y) and FerzinsNum: ";
	cin >> size_x >> size_y >> ferzinsNum;
	cin.get();

	cout << endl << "You are playing on " << size_x << "x" << size_y << " field with " << ferzinsNum << " ferzin(s)! " << endl << endl;

	class ferzin {
	public:
		int x, y;

		ferzin(int x, int y) {
			this->x = x;
			this->y = y;
		}

		void out() {
			cout << this->x << " " << this->y << endl;
		}
	};

	
	vector<ferzin> tempFerzins, ferzins;

	for (int i = 0; i < ferzinsNum; i++) {
		int x, y;
		cout << "Enter " << i+1 << " ferzin's X and Y: ";
		//cin >> ferzins[i].x >> ferzins[i].y;
		cin >> x >> y;
		cin.get();
		if (x > size_x || y > size_y) {
			cout << "[ ERROR ]: couldn't write " << (x > size_x ? "X:" : "Y: ") << (x > size_x ? x : y) << " > " << size_x << " or " << size_y << " . Try again: " << endl;
			i--;
			continue;
		}
		tempFerzins.push_back(ferzin(x, y));
	}
	cout << endl;

	for (int i = 0; i < tempFerzins.size(); i++) {
		for (int j = i + 1; j < tempFerzins.size(); j++) {
			if (tempFerzins[i].x == tempFerzins[j].x && tempFerzins[i].y == tempFerzins[j].y) {
				tempFerzins.erase(tempFerzins.begin() + j - 1);
				j--;
			}
		}
	}

	while (!tempFerzins.empty()) {
		int sum = 0, minFerz = -1;
		for (int i = 0; i < tempFerzins.size(); i++) {
			if (sum < tempFerzins[i].x + tempFerzins[i].y) {
				sum = tempFerzins[i].x + tempFerzins[i].y;
				minFerz = i;
			}
		}

		ferzins.push_back(tempFerzins[minFerz]);
		tempFerzins.erase(tempFerzins.begin() + minFerz);
	}

	reverse(ferzins.begin(), ferzins.end());
	
	cout << endl;
	for (int i = 0; i < ferzins.size(); i++) {
		cout << "Ferzin #" << i+1 << ": " << ferzins[i].x << " " << ferzins[i].y << endl;
	}
	cout << endl;
	

	bool war = false;
	for (int i = 0; i < ferzins.size(); i++) {
		bool upRight = false, downLeft = false, upLeft = false, downRight = false, up = false, down = false, right = false, left = false;

		for (int j = i + 1; j < ferzins.size(); j++) {
			bool found = false;

			for (int x = ferzins[i].x, y = ferzins[i].y; x <= size_x && y <= size_y && !found && !upRight; x++, y++) { //UpRight
				if (ferzins[j].x == x && ferzins[j].y == y) {
					cout << "WAR: " << i + 1 << " <=> " << j + 1 << endl;
					found = true;
					upRight = true;
					//cout << "UpRight" << endl;
				}
			}

			for (int x = ferzins[i].x, y = ferzins[i].y; x > 0 && y > 0 && !found && !downLeft; x--, y--) { //DownLeft
				if (ferzins[j].x == x && ferzins[j].y == y) {
					cout << "WAR: " << i + 1 << " <=> " << j + 1 << endl;
					found = true;
					downLeft = true;
					//cout << "DownLeft" << endl;
				}
			}

			for (int x = ferzins[i].x, y = ferzins[i].y; x > 0 && y <= size_y && !found && !upLeft; x--, y++) { //UpLeft
				if (ferzins[j].x == x && ferzins[j].y == y) {
					cout << "WAR: " << i + 1 << " <=> " << j + 1 << endl;
					found = true;
					upLeft = true;
					//cout << "UpLeft" << endl;
				}
			}

			for (int x = ferzins[i].x, y = ferzins[i].y; x <= size_x && y > 0 && !found && !downRight; x++, y--) { //DownRight
				if (ferzins[j].x == x && ferzins[j].y == y) {
					cout << "WAR: " << i + 1 << " <=> " << j + 1 << endl;
					found = true;
					downRight = true;
					//cout << "DownRight" << endl;
				}
			}

			for (int x = ferzins[i].x, y = ferzins[i].y; x <= size_x && !found && !right; x++) { //Right
				if (ferzins[j].x == x && ferzins[j].y == y) {
					cout << "WAR: " << i + 1 << " <=> " << j + 1 << endl;
					found = true;
					right = true;
					//cout << "Right" << endl;
				}
			}

			for (int x = ferzins[i].x, y = ferzins[i].y; x > 0 && !found && !left; x--) { //Left
				if (ferzins[j].x == x && ferzins[j].y == y) {
					cout << "WAR: " << i + 1 << " <=> " << j + 1 << endl;
					found = true;
					left = true;
					//cout << "Left" << endl;
				}
			}

			for (int x = ferzins[i].x, y = ferzins[i].y; y <= size_y && !found && !up; y++) { // Up
				if (ferzins[j].x == x && ferzins[j].y == y) {
					cout << "WAR: " << i + 1 << " <=> " << j + 1 << endl;
					found = true;
					up = true;
					//cout << "Up" << endl;
				}
			}

			for (int x = ferzins[i].x, y = ferzins[i].y; y > 0 && !found && !down; y--) { // Down
				if (ferzins[j].x == x && ferzins[j].y == y) {
					cout << "WAR: " << i + 1 << " <=> " << j + 1 << endl;
					found = true;
					down = true;
					//cout << "Down" << endl;
				}
			}

			if (found) war = true;

		}
	}

	if (!war) cout << endl << "All in PEACE <3" << endl;

	



}

void task6() {
	class dot {
	public:
		int x, y;

		dot(int x, int y) {
			this->x = x;
			this->y = y;
		}

		dot() {

		}
	};

	class Segment {
	public:
		dot start, end;

		Segment(dot start, dot end) {
			this->start = start;
			this->end = end;
		}

		Segment() {
		
		}
	};

	dot keyDot;
	Segment segment;
	bool crosses = false;
	cout << "Enter keyDot (x, y): ";
	cin >> keyDot.x >> keyDot.y;
	cin.get();

	cout << "Enter Segment Start (x, y) & End (x, y): ";
	cin >> segment.start.x >> segment.start.y >> segment.end.x >> segment.end.y;
	cin.get();
	cout << endl;

	if (segment.start.y < keyDot.y && keyDot.y < segment.end.y) { //y1 < y < y2

		if (keyDot.x < segment.end.x)
		crosses = true;

	}

	if (keyDot.y == segment.start.y || keyDot.y == segment.end.y) { //y1 = y || y2 = y
		if (keyDot.y == segment.start.y && keyDot.y == segment.end.y && (keyDot.x < segment.end.x || keyDot.x < segment.start.x)) {//y = y1 = y2, x < x1 || x < x2
			crosses = true;
		}

		if ((keyDot.x == segment.start.x && keyDot.y == segment.start.y) || (keyDot.x == segment.end.x && keyDot.y == segment.end.y)) {
			crosses = true;
			cout << "keyDot equal segment's dot" << endl;
		}

	}

	if (crosses) {
		cout << "Beam from KeyDot [ CROSSES ] the Segment!" << endl;
	}
	else {
		cout << "Beam from KeyDot [ DOESN'T cross ] the Segment!" << endl;
	}

}

int main()
{
	task5();

}

