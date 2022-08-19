
#pragma once
#include "classes/classes.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>
#include <cmath>
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"


using std::cout;
using std::endl;
using std::fstream;
using std::string;

vector<Plane> readPlanes(string planes);
vector<Route> readRoutes(string routes);
vector<Airline> readAirlines(string airlines);
vector<Airport> readAirports(string airports);
Graph makeGraph(string routesFile,string airportsFile,std::map<string,int> &nameMap);
bool empty(Airport air);

