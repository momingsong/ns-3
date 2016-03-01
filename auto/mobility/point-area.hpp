#pragma once
#include <vector>
class Point
{
public:
	Point();
	Point(double x, double y);
	bool operator==(Point x);
	double x;
	double y;
};

inline Point::Point()
{
	this->x = 0;
	this->y = 0;
}

inline Point::Point(double x, double y)
{
	this->x = x;
	this->y = y;
}

inline bool Point::operator==(Point a)
{
	if (this->x == a.x && this->y == a.y) return true;
	else return false;
}
typedef std::vector<Point*> Points;

class Area
{
public:
	Area();
	Area(double ltx, double lty, double rbx, double rby);
	Point ltCorner;
	Point rbCorner;
};

inline Area::Area()
{
	ltCorner = Point();
	rbCorner = Point();
}

inline Area::Area(double ltx, double lty, double rbx, double rby)
{
	ltCorner = Point(ltx, lty);
	rbCorner = Point(rbx, rby);
}
