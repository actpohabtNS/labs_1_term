#include <iostream>
#include <stack> 
#include <stdlib.h> 
#include <vector>
#include <math.h>

using namespace std;

struct Point {
	double x, y;
};

Point p0;

Point nextToTop(stack<Point>& S)
{
	Point p = S.top();
	S.pop();
	Point res = S.top();
	S.push(p);
	return res;
}
 
void swap(Point& p1, Point& p2)
{
	Point temp = p1;
	p1 = p2;
	p2 = temp;
}

double distSq(Point p1, Point p2)
{
	return (p1.x - p2.x) * (p1.x - p2.x) +
		(p1.y - p2.y) * (p1.y - p2.y);
}
 
int orientation(Point p, Point q, Point r)
{
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // colinear 
	return (val > 0) ? 1 : 2; // clock or counterclock wise 
}


int compare(const void* vp1, const void* vp2)
{
	Point* p1 = (Point*)vp1;
	Point* p2 = (Point*)vp2;

	// Find orientation 
	int o = orientation(p0, *p1, *p2);
	if (o == 0)
		return (distSq(p0, *p2) >= distSq(p0, *p1)) ? -1 : 1;

	return (o == 2) ? -1 : 1;
}

stack<Point> convexHull(Point points[], int n) {
	int ymin = points[0].y, min = 0;

	for (int i = 1; i < n; i++) {
		int y = points[i].y;

		if ((y < ymin) || (ymin == y &&
			points[i].x < points[min].x))
			ymin = points[i].y, min = i;
	}

	swap(points[0], points[min]);

	p0 = points[0];
	qsort(&points[1], n - 1, sizeof(Point), compare);

	int m = 1;
	for (int i = 1; i < n; i++) {
		while (i < n - 1 && orientation(p0, points[i],
			points[i + 1]) == 0)
			i++;


		points[m] = points[i];
		m++; 
	}


	stack<Point> S;

	if (m < 3) return S;
 
	S.push(points[0]);
	S.push(points[1]);
	S.push(points[2]);

	for (int i = 3; i < m; i++)
	{
		while (orientation(nextToTop(S), S.top(), points[i]) != 2)
			S.pop();
		S.push(points[i]);
	}

	return S;

	/*
	while (!S.empty())
	{
		Point p = S.top();
		cout << "(" << p.x << ", " << p.y << ")" << endl;
		S.pop();
	}
	*/
}

int main()
{
	int pointNums;

	cout << "Enter -=[ Number of Points ]=- : ";
	cin >> pointNums;

	Point* points = new Point[pointNums];

	for (int i = 0; i < pointNums; i++) {
		double x, y;
		cout << "Enter -=[ " << i + 1 << " point ]=- coords (x, y): ";
		cin >> x >> y;

		points[i] = { x, y };
	}

	stack<Point> hullTemp = convexHull(points, pointNums);

	vector<Point> hull;

	while (!hullTemp.empty()) {
		hull.push_back(hullTemp.top());
		hullTemp.pop();
	}

	double maxDist = 0, maxSumDist = 0;
	int max1, max2;
	Point furthest1, furthest2, furthest3;

	for (int i = 0; i < hull.size(); i++) {
		for (int j = i + 1; j < hull.size(); j++) {
			if (maxDist < pow(distSq(hull[i], hull[j]), 0.5)) {
				maxDist = pow(distSq(hull[i], hull[j]), 0.5);
				max1 = i; furthest1 = hull[i];
				max2 = j; furthest2 = hull[j];
			}
		}
	}

	cout << endl << "Furthest points: ( " << furthest1.x << ", " << furthest1.y << " ) , ( " << furthest2.x << ", " << furthest2.y << " )" << endl;

	hull.erase(hull.begin() + max1);
	hull.erase(hull.begin() + max2 - 1);

	if (hull.size() == 1) {
		furthest3 = hull[0];
	}
	else {
		for (int i = 0; i < hull.size(); i++) {
			if (maxSumDist < +distSq(furthest1, hull[i]) + +distSq(furthest2, hull[i])) {
				maxSumDist = pow(distSq(furthest1, hull[i]), 0.5) + pow(distSq(furthest2, hull[i]), 0.5);
				furthest3 = hull[i];
			}
		}
	}

	cout << endl << "Third point: ( " << furthest3.x << " , " << furthest3.y << " )" << endl;

	double a = pow(distSq(furthest1, furthest2), 0.5), b = pow(distSq(furthest2, furthest3), 0.5), c = pow(distSq(furthest1, furthest3), 0.5);
	double p = (a + b + c) / 2;
	double radius = (a * b * c) / (4 * pow(p * (p - a) * (p - b) * (p - c), 0.5));

	cout << endl << "Radius: " << radius << endl;

	return 0;
}
