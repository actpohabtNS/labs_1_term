#include <iostream>
#include <vector>

using namespace std;

class dot {
public:
	int x, y;

	dot(int x, int y) {
		this->x = x;
		this->y = y;
	}

	dot() = default;
};

bool operator == (dot d1, dot d2) {
	return (d1.x == d2.x && d1.y == d2.y);
}

class Segment {
public:
	dot start, end;

	Segment(dot start, dot end) {
		this->start = start;
		this->end = end;
	}

	Segment() = default;

	void out() {
		cout << "start( " << this->start.x << ", " << this->start.y << " ), end(  " << this->end.x << ", " << this->end.y << " )" << endl;
	}

};

int dotCrossesSeg(dot kD, Segment s) {
	dot s1 = s.start;
	dot s2 = s.end;

	if (s1.y <= kD.y && kD.y <= s2.y && ((s2.y - s1.y) * (kD.x - s2.x) - (kD.y - s2.y) * (s2.x - s1.x) < 0)) return 1;
	if (s1.y <= kD.y && kD.y <= s2.y && ((s2.y - s1.y) * (kD.x - s2.x) - (kD.y - s2.y) * (s2.x - s1.x) == 0)) return 2;

	if (s2.y <= kD.y && kD.y <= s1.y && ((s2.y - s1.y) * (kD.x - s2.x) - (kD.y - s2.y) * (s2.x - s1.x) > 0)) return 1;
	if (s2.y <= kD.y && kD.y <= s1.y && ((s2.y - s1.y) * (kD.x - s2.x) - (kD.y - s2.y) * (s2.x - s1.x) == 0)) return 2;
	
	return 0;
}


int main()
{
	dot keyDot;
	int vertexesNum = 0;
	vector<Segment> segments;
	vector<dot> dots;
	cout << "Enter -=[ keyDot ]=- coords (x, y), -=[ Num of Vertexes ]=- : ";
	cin >> keyDot.x >> keyDot.y >> vertexesNum;
	cin.get();

	for (int i = 0; i < vertexesNum; i++) {
		int x, y;
		cout << "Enter -=[ " << i + 1 << "'s dot ]=- coords (x, y): ";
		cin >> x >> y;
		cin.get();

		dots.push_back(dot(x, y));

		if (i >= 1) {
			segments.push_back(Segment(dots[i-1], dots[i]));
		}
	}

	segments.push_back(Segment(dots[dots.size()-1], dots[0]));
	
	/*
	cout << "Your segments: " << endl;
	for (Segment seg : segments) {
		seg.out();
		cout << (dotCrossesSeg(keyDot, seg) ? "crosses" : "Does NOT cross") << endl;
	}
	*/

	int crosses = 0;
	for (Segment seg : segments) {
		if (dotCrossesSeg(keyDot, seg) == 1) {
			crosses++;
		} else if (dotCrossesSeg(keyDot, seg) == 2) {
			crosses = 1;
			break;
		}

	}

	cout << endl << "Crosses: " << crosses << endl;
	if (crosses % 2 == 1) {
		cout << "KeyDot is -=[ INSIDE ]=- the Polygon!" << endl;
	}
	else {
		cout << "KeyDot is -=[ NOT inside ]=- the Polygon!" << endl;
	}
}
