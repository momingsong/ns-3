#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <algorithm>
#include <bitset>
#include "point-area.hpp"
#include "group.hpp"
#include "nodes.hpp"
#include "utilities.hpp"
#include "./XML/tinyxml.h"

const size_t bitsetsize = 36000;

#define DEBUG
void GenerateTrace();
void AnalyzeTrace(char* path, double timeSlot, double totalTime, double timeStamp, int monitorNode, double endTimestamp, double range);
void ContinueMoving(Node* movingNode, double timeSlot, double totalTime);

int main(int argc, char** argv)
{
	//GenerateTrace();
	if (argc <= 1)
		GenerateTrace();
	else
	{
		if (argc != 8)
			std::cout << "Usage: traceFilePath observationInterval duration startTime consumerIdx endTime, commnunicationRange\n";
		else
			AnalyzeTrace(argv[1], atof(argv[2]), atof(argv[3]), atof(argv[4]), atoi(argv[5]), atof(argv[6]), atof(argv[7]));
	}
}


void GenerateTrace()
{
	srand((size_t)(time(NULL)));
	Groups groups = Groups();
	Individuals indvs = Individuals();
	Nodes ns = Nodes();
	//For future read, for groupnumber and parameters
	TiXmlDocument para("./Parameter/p.xml");
	para.LoadFile();


	/*
	if (para.ErrorId() > 0)
	{
	std::cout << "Error!" << std::endl;
	system("pause");
	return 0;
	}
	*/
	TiXmlElement *root = para.RootElement();
	double timeslot = atof(root->Attribute("timeslot"));
	double duration = atof(root->Attribute("duration"));
	TiXmlElement *groupsElem = root->FirstChildElement("groups");
	int countNodes = 0;

	int gn = 0;
	for (TiXmlElement *groupElem = groupsElem->FirstChildElement(); groupElem; groupElem = groupElem->NextSiblingElement())
	{
		Group* g = new Group(
			atof(groupElem->Attribute("pLeaving")),
			atof(groupElem->Attribute("pJoin")),
			atof(groupElem->Attribute("pDrift")),
			atof(groupElem->Attribute("pWholeMove")),
			0,
			atof(groupElem->Attribute("maxSpeed")),
			atof(groupElem->Attribute("joinSpeed"))
			);
		g->groupArea = new Area(
			atof(groupElem->FirstChildElement("area")->Attribute("ltx")),
			atof(groupElem->FirstChildElement("area")->Attribute("lty")),
			atof(groupElem->FirstChildElement("area")->Attribute("rbx")),
			atof(groupElem->FirstChildElement("area")->Attribute("rby"))
			);

		if (groupElem->FirstChildElement("entry") != NULL)
		{
			g->groupEntry = new Points();
			for (TiXmlElement *entrypoint = groupElem->FirstChildElement("entry")->FirstChildElement("point"); entrypoint; entrypoint = entrypoint->NextSiblingElement())
				g->groupEntry->push_back(new Point(atof(entrypoint->Attribute("x")), atof(entrypoint->Attribute("y"))));
		}
		if (groupElem->FirstChildElement("whole") != NULL)
		{
			g->groupMoveSequence = new Points();
			for (TiXmlElement *entrypoint = groupElem->FirstChildElement("whole")->FirstChildElement("point"); entrypoint; entrypoint = entrypoint->NextSiblingElement())
				g->groupMoveSequence->push_back(new Point(atof(entrypoint->Attribute("x")), atof(entrypoint->Attribute("y"))));
		}
		groups.push_back(g);


		int nodeNumber = atoi(groupElem->FirstChildElement("nodes")->Attribute("number"));
		for (int i = 0; i < nodeNumber; i++)
		{
			Node* n = new Node(
				gn,
				doublerand()*(groups[gn]->groupArea->rbCorner.x - groups[gn]->groupArea->ltCorner.x) + groups[gn]->groupArea->ltCorner.x,
				doublerand()*(groups[gn]->groupArea->rbCorner.y - groups[gn]->groupArea->ltCorner.y) + groups[gn]->groupArea->ltCorner.y
				);
			std::cout << "$node(" << countNodes << ") set X_ " << n->location.x << std::endl;
			std::cout << "$node(" << countNodes++ << ") set Y_ " << n->location.y << std::endl;
			ns.push_back(n);
		}
		g->pDrift /= nodeNumber;
		g->pLeaving /= nodeNumber;

		gn++;
	}

	int in = 1000;
	TiXmlElement *indvsElem = root->FirstChildElement("individuals");
	for (TiXmlElement *indvElem = indvsElem->FirstChildElement(); indvElem; indvElem = indvElem->NextSiblingElement())
	{
		Individual* i = new Individual(
			atof(indvElem->Attribute("pDrift")),
			atof(indvElem->Attribute("driftRange")),
			atof(indvElem->Attribute("maxSpeed")),
			atof(indvElem->Attribute("maxSpeed"))
			);
		indvs.push_back(i);

		for (int j = 0; j < atoi(indvElem->FirstChildElement("nodes")->Attribute("number")); j++)
		{
			//Node* n = new Node(
			//	in,
			//	doublerand()*i->ratioDrift * 15 - doublerand()*i->ratioDrift * 15, doublerand()*i->ratioDrift * 15 - doublerand()*i->ratioDrift * 15);
			//0 - doublerand()*i->ratioDrift * 15, doublerand()*i->ratioDrift * 15 - doublerand()*i->ratioDrift*15);
			Node* n = new Node(in, -200, -200);
			std::cout << "$node(" << countNodes << ") set X_ " << n->location.x << std::endl;
			std::cout << "$node(" << countNodes++ << ") set Y_ " << n->location.y << std::endl;
			ns.push_back(n);
		}

		i->pDrift /= atoi(indvElem->FirstChildElement("nodes")->Attribute("number"));

		in++;
	}

	void GroupMove(Groups groups, Individuals individuals, Nodes nodes, double timeSlot, double totalTime);
	GroupMove(groups, indvs, ns, timeslot, duration);

}

void AnalyzeTrace(char* path, double timeSlot, double totalTime, double timeStamp, int monitorNode,  double endTimestamp, double range)
{
	int durationbit = (int)round((endTimestamp - timeStamp) / timeSlot);

	std::ifstream ifs(path);
	MoveingNodes m;
	std::string line;
	Nodes traceNodes = Nodes();

	int nodenumber;
	double moveTime, destxx, destyy, destv;
	std::string temp, nodeBrack;
	
	while (std::getline(ifs, line))
	{
		if (line[0] == '#') continue;
		else if (line[2] == 'o') 
		{
			size_t xpos = line.find_first_of('X');
			double xx = atof(line.substr(xpos + 2).c_str());
			std::getline(ifs, line);
			size_t ypos = line.find_first_of('Y');
			double yy = atof(line.substr(ypos + 2).c_str());
			Node* n = new Node(atoi(line.substr(6, xpos - 6).c_str()), xx, yy);
			traceNodes.push_back(n);
		}
		else if (line[2] == 's') 
		{
			std::stringstream ss(line);
			ss >> temp >> temp>> moveTime >> nodeBrack >> temp >> destxx >> destyy >> destv;
			int nodeIndex = atoi(nodeBrack.substr(6, nodeBrack.size() - 7).c_str());
			//std::cout << nodeIndex << std::endl;
			Sequence* s = new Sequence(moveTime, traceNodes[nodeIndex], destxx, destyy, destv);
			m.push_back(s);
		}
	}
	std::sort(m.begin(), m.end(), Sequence::comp);
	double* d = new double[traceNodes.size()];
	bool* b = new bool[traceNodes.size()];
	double* dz = new double[traceNodes.size()];
	std::bitset<bitsetsize>** bitmatrix = new std::bitset<bitsetsize>*[traceNodes.size()];
	for (size_t i = 0; i < traceNodes.size(); i++)
	{
		dz[i] = 0;
		bitmatrix[i] = new std::bitset<bitsetsize>[traceNodes.size()];
	}
	int j = 0;
	int bitconnect = 0;
	for (double t = 0; t < totalTime; t += timeSlot)
	{
		if (t - m[j]->time >= -1e-6)
		{
			m[j]->node->SetDestination(m[j]->destination.x, m[j]->destination.y, m[j]->speed);
			if (j < m.size() - 1)j++;
		}
		for (size_t i = 0; i < traceNodes.size(); i++)
		{
			if (traceNodes[i]->speed > 1e-6)
				ContinueMoving(traceNodes[i], timeSlot, t);
		}
		if (t - timeStamp < 1e-6 && t - timeStamp > -1e-6 && endTimestamp < 1e-6)
		{

			for (size_t i = 0; i < traceNodes.size(); i++)
			{
				b[i] = distance(traceNodes[i]->location, traceNodes[monitorNode]->location) < range ? true : false;
				std::cout << b[i] << "\t";
			}
			std::cout << std::endl;

		}
		else if (t-timeStamp>-1e-6  && endTimestamp-t>1e-6)
		{
			//std::cout << t<< std::endl;
			for (size_t i = 0; i < traceNodes.size(); i++)
			{
				dz[i] = distance(traceNodes[i]->location, traceNodes[monitorNode]->location) < range ? dz[i] + timeSlot : dz[i];
			}
			for (size_t i = 0; i < traceNodes.size(); i++)
			{
				for (size_t j = 0; j < traceNodes.size(); j++) 
				{
					bitmatrix[i][j][bitconnect] = distance(traceNodes[i]->location, traceNodes[j]->location) < range ? 1 : 0;
					//if (bitmatrix[i][j].count() != 0)
					//	std::cout << i << " " << j << std::endl;
				}
			}
			bitconnect += 1;
		}
	}
	bool stop = true;
	int iterationtime = 0;
	while (stop)
	{
		stop = false;
		for (size_t i = 0; i < traceNodes.size(); i++)
		{
			for (size_t j = 0; j < traceNodes.size(); j++)
			{
				for (size_t k = 0; k < traceNodes.size(); k++)
				{
					std::bitset<bitsetsize> q = (bitmatrix[i][j] & bitmatrix[j][k]) | bitmatrix[i][k];
					if (bitmatrix[i][k] != q)
					{
						stop = true;
					}
					bitmatrix[i][k] = (bitmatrix[i][j] & bitmatrix[j][k]) | bitmatrix[i][k];
					
				}
			}
		}
		iterationtime += 1;
	}
	/*
	std::cout << iterationtime << std::endl;
	for (size_t i = 0; i < traceNodes.size(); i++)
	{
		std::cout << dz[i] << "\t";
	}
	std::cout<< std::endl;
	*/
	for (size_t j = 0; j < traceNodes.size(); j++)
	{
		bool flip = false;
		if (bitmatrix[monitorNode][j].any())
			std::cout<<j<<" ";
		for (size_t k = 0; k < bitmatrix[monitorNode][j].size(); k++)
		{
			if(bitmatrix[monitorNode][j][k]!=1 && flip)
			{
				std::cout<<(float)k*timeSlot<<" ";
				flip = false;
			}
			if(bitmatrix[monitorNode][j][k]==1 && !flip)
			{
				std::cout<<(float)k*timeSlot<<",";
				flip = true;
			}
		}
		if (bitmatrix[monitorNode][j].any())
			std::cout<<"\n";
		//std::cout << (double)bitmatrix[monitorNode][j].count()*timeSlot<<"\n";
	}
	//std::cout << std::endl;
}


void ContinueMoving(Node* movingNode, double timeSlot, double totalTime)
{
	double distance = sqrt(pow(movingNode->destination.x - movingNode->location.x, 2) + pow(movingNode->destination.y - movingNode->location.y, 2));
	double speedX = movingNode->speed * (movingNode->destination.x - movingNode->location.x) / distance;
	double speedY = movingNode->speed * (movingNode->destination.y - movingNode->location.y) / distance;
	if (abs(movingNode->location.x + speedX*timeSlot - movingNode->destination.x) > abs(movingNode->location .x - movingNode->destination.x)
		|| abs(movingNode->location.y + speedY*timeSlot - movingNode->destination.y) > abs(movingNode->location.y - movingNode->destination.y))
	{
		movingNode->location.x = movingNode->destination.x;
		movingNode->location.y = movingNode->destination.y;
		movingNode->speed = 0;
		//std::cout << "#Finish at "<<movingNode->location.x << " " << movingNode->location.y<<" "<<totalTime<<" "<<movingNode->group<<std::endl;
	}
	else
	{
		movingNode->location.x += speedX*timeSlot;
		movingNode->location.y += speedY*timeSlot;
		//std::cout << "#Inline" << movingNode->location.x << " " << movingNode->location.y << std::endl;
	}
}
void GroupMove(Groups groups, Individuals individuals, Nodes nodes, double timeSlot, double totalTime)
{
	int coinFliping = 0;
	int fDrift, fLeaving, fJoin;
	int fWholeMove;
	//Each time slots
	for (double t = 0; t < totalTime; t += timeSlot)
	{
		for (size_t i = 0; i < groups.size(); i++)
		{
			Group* g = groups[i];
			if (g->groupMoveSequence != NULL && g->groupMoveSequence->size()>0)
			{
				fWholeMove = hugerand();
				if (g->pWholeMove / timeSlot * totalTime> fWholeMove)
				{
					Point* lNow = NULL;
					Point* lNext = NULL;
					for (size_t j = 0; j < g->groupMoveSequence->size(); j++)
					{
						if (*g->groupMoveSequence->at(j) == g->groupArea->ltCorner && j != g->groupMoveSequence->size() - 1)
						{
							lNow = g->groupMoveSequence->at(j);
							lNext = g->groupMoveSequence->at(j + 1);
							g->groupArea->rbCorner.x = g->groupArea->rbCorner.x - g->groupArea->ltCorner.x + lNext->x;
							g->groupArea->rbCorner.y = g->groupArea->rbCorner.y - g->groupArea->ltCorner.y + lNext->y;
							g->groupArea->ltCorner.x = lNext->x; 
							g->groupArea->ltCorner.y = lNext->y;
							break;
						}
					}
					if (lNow != NULL)
					{
						double difxx = lNext->x - lNow->x;
						double difyy = lNext->y - lNow->y;
						double difdis = sqrt(difxx*difxx + difyy*difyy);
						for (size_t k = 0; k < nodes.size(); k++)
						{
							if (nodes[k]->group == i)
							{
								nodes[k]->destination.x += difxx;
								nodes[k]->destination.y += difyy;
								double sx = g->maxSpeed * difxx / difdis;
								double sy = g->maxSpeed * difyy / difdis;
								double distance = sqrt(pow(nodes[k]->destination.x - nodes[k]->location.x, 2) + pow(nodes[k]->destination.y - nodes[k]->location.y, 2));
								double speedX = nodes[k]->speed * (nodes[k]->destination.x - nodes[k]->location.x) / distance + sx;
								double speedY = nodes[k]->speed * (nodes[k]->destination.y - nodes[k]->location.y) / distance + sy;
								double speed = sqrt(speedX*speedX + speedY*speedY);
								nodes[k]->speed = speed;
								std::cout << "$ns_ at " << t << " $node(" << k << ") setdest " << nodes[k]->destination.x << " " << nodes[k]->destination.y << " " << nodes[k]->speed << std::endl << "#WholeMove\n";
							}
						}
					}
				}
			}
		}
		//Each nodes
		for (Nodes::iterator nodep = nodes.begin(); nodep != nodes.end(); nodep++)
		{
			Node* node = *nodep;
			//Generate the random number
			fDrift = hugerand();
			fLeaving = hugerand();
#ifdef DEBUG
			coinFliping++;
#endif
			//if the node is not moving
			if (node->speed < 1e-7)
			{
				//If the node in the group
				if (node->group < 1000)
				{
					Group* g = groups[node->group];
					//Decide the leaving event
					if (g->pLeaving * timeSlot / totalTime * HUGE_RAND_MAX > fLeaving)
					{
						int c = randomIndex(individuals.size());
						node->group = c + 1000;
						//std::pair<double, double> out = outside(g->groupArea->ltCorner.x, g->groupArea->rbCorner.x, g->groupArea->ltCorner.y, g->groupArea->rbCorner.y);
						//for pushing the consumer to the mininum
						std::pair<double, double> out(-200, -200);
						//If the group have entry
						if (g->groupEntry != NULL && g->groupEntry->size()>0)
						{
							Point* p = g->groupEntry->at(randomIndex(g->groupEntry->size()));
							node->SetDestination(p->x, p->y, g->joinSpeed);
							double tt = sqrt(pow(node->location.x - p->x, 2) + pow(node->location.y - p->y, 2)) / g->joinSpeed;
							std::cout << "$ns_ at " << t << " $node(" << nodep - nodes.begin() << ") setdest " << node->destination.x << " " << node->destination.y << " " << node->speed << std::endl << "#leave\n";
							node->SetDestination(out.first, out.second, g->joinSpeed);
							std::cout << "$ns_ at " << t + tt << " $node(" << nodep - nodes.begin() << ") setdest " << node->destination.x << " " << node->destination.y << " " << node->speed << std::endl << "#leave\n";
						}
						//If the group is open
						else
						{
							node->SetDestination(out.first, out.second, g->joinSpeed);
							std::cout << "$ns_ at " << t << " $node(" << nodep - nodes.begin() << ") setdest " << node->destination.x << " " << node->destination.y << " " << node->speed << std::endl << "#leave\n";
						}

					}
					//Decide the drift event
					else if (g->pDrift * timeSlot / totalTime * HUGE_RAND_MAX > fDrift)
					{
#ifdef DEBUG
						coinFliping += 1 << 20;
#endif
						double driftspeed = g->maxSpeed * doublerand();
						double xx = g->groupArea->ltCorner.x + (g->groupArea->rbCorner.x - g->groupArea->ltCorner.x) * doublerand();
						double yy = g->groupArea->ltCorner.y + (g->groupArea->rbCorner.y - g->groupArea->ltCorner.y) * doublerand();
						node->SetDestination(xx, yy, driftspeed);
						std::cout << "$ns_ at " << t << " $node(" << nodep - nodes.begin() << ") setdest " << node->destination.x << " " << node->destination.y << " " << node->speed << std::endl << "#drift\n";
					}
				}
				//If the node is individual
				else
				{
					Individual* i = individuals[node->group - 1000];
					//Decide individual drift event
					if (i->pDrift * timeSlot / totalTime * HUGE_RAND_MAX > fDrift)
					{
						double driftspeed = i->maxSpeed * doublerand();
						double xx = node->location.x + i->ratioDrift * doublerand();
						double yy = node->location.y + i->ratioDrift * doublerand();
						node->SetDestination(xx, yy, driftspeed);
						std::cout << "$ns_ at " << t << " $node(" << nodep - nodes.begin() << ") setdest " << node->destination.x << " " << node->destination.y << " " << node->speed << std::endl << "#Idrift\n";
					}
					//Decide join event
					else
					{
						for (size_t gp = 0; gp < groups.size(); gp++)
						{
							fJoin = hugerand();
							if (groups[gp]->pJoin * timeSlot / totalTime * HUGE_RAND_MAX > fJoin)
							{

								node->group = gp;
								double xx = (groups[gp]->groupArea->rbCorner.x - groups[gp]->groupArea->ltCorner.x) * doublerand() + groups[gp]->groupArea->ltCorner.x;
								double yy = (groups[gp]->groupArea->rbCorner.y - groups[gp]->groupArea->ltCorner.y) * doublerand() + groups[gp]->groupArea->ltCorner.y;
								//If the group have entry, must enter the entry
								if (groups[gp]->groupEntry != NULL && groups[gp]->groupEntry->size() > 0)
								{
									Point* p = groups[gp]->groupEntry->at(randomIndex(groups[gp]->groupEntry->size()));
									node->SetDestination(p->x, p->y, groups[gp]->joinSpeed);
									double tt = sqrt(pow(node->location.x - p->x, 2) + pow(node->location.y - p->y, 2)) / groups[gp]->joinSpeed;
									std::cout << "$ns_ at " << t << " $node(" << nodep - nodes.begin() << ") setdest " << node->destination.x << " " << node->destination.y << " " << node->speed << std::endl << "#join\n";
									node->SetDestination(xx, yy, groups[gp]->joinSpeed);
									std::cout << "$ns_ at " << t + tt << " $node(" << nodep - nodes.begin() << ") setdest " << node->destination.x << " " << node->destination.y << " " << node->speed << std::endl << "#join\n";
								}
								//If the group is open
								else 
								{
									node->SetDestination(xx, yy, groups[gp]->joinSpeed);
									std::cout << "$ns_ at " << t << " $node(" << nodep - nodes.begin() << ") setdest " << node->destination.x << " " << node->destination.y << " " << node->speed << std::endl << "#join\n";
								}
								break;
							}
							
						}
					}
				}
			}
			else
			{
				ContinueMoving(node, timeSlot, t);
			}
		}
		/*
		//Each group
		for (int i = 0; i < groupNumber; i++)
		{
			Group currentGroup = groups[i];
			//Each node in group
			for (int j = 0; j < currentGroup->size; j++)
			{
				//generate random numbers
				fDrift = (double)rand() / RAND_MAX;
				fLeaving = (double)rand() / RAND_MAX;
				Node currentNode = currentGroup->nodes[j];
				//std::cout << "current speed" << currentNode->speed<<std::endl;
				//if the node is not moving
				if (currentNode->speed <1e-7 && currentNode->speed > -1e-7)
				{
					//2 situations: drift or leaving
					//All the 2 situations will have outputs. Others will not, except initialize.
					if (fDrift < currentGroup->pDrift)
					{
						//determine speed and drift ratio
						//std::cout << "Drift: " << j << "\t";
						currentNode->speed = (double)rand() / RAND_MAX * currentGroup->maxSpeed *currentGroup->ratioDrift;
						currentNode->destination.x = currentNode->location.x + doublerand() * (currentGroup->groupArea.ltCorner.x - currentGroup->groupArea.rbCorner.x)*currentGroup->ratioDrift;
						currentNode->destination.y = currentNode->location.y + doublerand() * (currentGroup->groupArea.ltCorner.y - currentGroup->groupArea.rbCorner.y)*currentGroup->ratioDrift;
						//std::cout << currentNode->destination.x <<"\t"<< currentNode->destination.y << "\t" << currentNode->speed << std::endl;
						
					}
					else if (fLeaving < currentGroup->pLeaving)
					{
						if (currentGroup->entry != NULL)
						{
							for (int i = 0; i < currentGroup->entryNumber; i++)
							{

							}
						}
						else
						{
							//std::cout << "Out: " << j << "\n";
							double xout = doublerand()*(currentGroup->groupArea.rbCorner.x - currentGroup->groupArea.ltCorner.x) + currentGroup->groupArea.ltCorner.x;
							double yout = doublerand()*(currentGroup->groupArea.rbCorner.y - currentGroup->groupArea.ltCorner.y) + currentGroup->groupArea.ltCorner.y;
							currentNode->destination.x = xout;
							currentNode->destination.y = yout;
							currentNode->speed = currentGroup->maxSpeed*doublerand();
							//std::cout << currentNode->destination.x << "\t" << currentNode->destination.y << "\t" << currentNode->speed << std::endl;
							
						}
					}
				}
				//if the node is already moving: continue moving and refresh the position
				else
				{
					ContinueMoving(currentNode, timeSlot);
				}
			}
		}

		*/
		
	}
	std::cout << (coinFliping&1048575)<<" "<<(coinFliping>>20) << std::endl;
}
