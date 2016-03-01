#pragma once
#include "point-area.hpp"

class Node
{
public:
	int group;
	Point location;
	Point destination;
	double speed;
	Node(int group, double x, double y);
	void SetDestination(double x, double y, double speed);
private:

};

Node::Node(int group, double x, double y)
{
	this->group = group;
	this->location.x = x;
	this->location.y = y;
	this->destination.x = x;
	this->destination.y = y;
	this->speed = 0;
}

inline void Node::SetDestination(double x, double y, double speed)
{
	this->destination.x = x;
	this->destination.y = y;
	this->speed = speed;
}

typedef std::vector<Node*> Nodes;

class Sequence
{
public:
	double time;
	Node* node;
	Point destination;
	double speed;

	Sequence(double time, Node* nodes, double x, double y, double speed);
	~Sequence();
	static bool comp(Sequence *a, Sequence *b);

private:

};

inline Sequence::Sequence(double time, Node* node, double x, double y, double speed)
{
	this->time = time;
	this->node = node;
	this->destination.x = x;
	this->destination.y = y;
	this->speed = speed;
}

inline Sequence::~Sequence()
{
}

inline bool Sequence::comp(Sequence * a, Sequence * b)
{
	return a->time < b->time;
}

typedef std::vector<Sequence*> MoveingNodes;