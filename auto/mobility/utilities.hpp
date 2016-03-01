#pragma once
#include <cstdlib>
#include <cmath>
#include <utility>
#define PI 3.1415926535898
#define HUGE_RAND_MAX 1073741823
inline double doublerand()
{
	return (double)rand() / (double)RAND_MAX;
}
inline int hugerand()
{
	return rand();
}
inline int randomIndex(int size)
{
	int t = (int)(size*doublerand());
	return t == size ? t - 1 : t;
}
inline std::pair<double, double> outside(double x1, double x2, double y1, double y2)
{
	double radius = sqrt(pow(x2 - x1, 2)*0.25 + pow(y2 - y1, 2)*0.25);
	double rad = doublerand() * 2 * PI;
	double xx = radius*sin(rad)*(doublerand()*0.5 + 1) + 0.5 * (x2 - x1);
	double yy = radius*cos(rad)*(doublerand()*0.5 + 1) + 0.5 * (y2 - y1);
	return std::pair<double, double>(xx, yy);
}

inline double distance(Point a, Point b)
{
	return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}