#pragma once
#include "point-area.hpp"

class Group
{
public:
	int id;
	Area *groupArea;
	double pLeaving;
	double pJoin;
	double pDrift;
	double pWholeMove;
	double ratioDrift;
	Points *groupMoveSequence;
	double maxSpeed;
	double joinSpeed;
	Point* groupCenter;

	//Temp
	Points *groupEntry;

	Group(double pLeaving, double pJoin, double pDrift, double pWholeMove, double ratioDrift, double maxSpeed, double joinSpeed);
	~Group();
};

Group::Group(double pLeaving, double pJoin, double pDrift, double pWholeMove, double ratioDrift, double maxSpeed, double joinSpeed)
{
	this->pLeaving = pLeaving;
	this->pJoin = pJoin;
	this->pDrift = pDrift;
	this->pWholeMove = pWholeMove;
	this->ratioDrift = ratioDrift;
	this->maxSpeed = maxSpeed;
	this->joinSpeed = joinSpeed;
	groupArea = NULL;
	groupMoveSequence = new Points();
	groupEntry = NULL;
	groupCenter = NULL;
}

inline Group::~Group()
{
	if (groupMoveSequence != NULL) delete(groupMoveSequence);
	if (groupArea != NULL) delete(groupArea);
	if (groupCenter != NULL) delete(groupCenter);
	if (groupEntry != NULL) delete(groupEntry);
}

typedef std::vector<Group*> Groups;

class Individual : public Group
{
public:
	Individual(double pDrift, double ratioDrift, double maxSpeed, double leaveSpeed); 
};

inline Individual::Individual(double pDrift, double ratioDrift, double maxSpeed, double leaveSpeed)
	:Group(0, 0, pDrift, 0, ratioDrift, maxSpeed, leaveSpeed)
{
	if (groupMoveSequence != NULL) delete(groupMoveSequence);
	if (groupArea != NULL) delete(groupArea);
	if (groupCenter != NULL) delete(groupCenter);
}

typedef std::vector<Individual*> Individuals;