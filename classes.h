#pragma once

#include <string>
#include <vector>
#include <map>


using std::string;
using std::vector;


class Plane{
 public:
  Plane();
  Plane(string name,string IATA,string ICAO);
 
  string name_;
  string IATA_;
  string ICAO_;
};


class Airline{
 public:
  Airline();
  Airline(int ID,string name,string alias, string IATA,string ICAO,string country,bool active);
  

  int ID_;
  string name_;
  string alias_;
  string IATA_;
  string ICAO_;
  string country_;
  bool active_;
};

class Route{
 public:
  Route();
  Route(int line,bool code,int source,int destination,int stops, vector<string> equipment);

  int line_;
  bool code_;
  int source_;
  int destination_;
  int stops_;
  vector<string> equipment_;
};
//changes made in airport class to better represent incoming and outgoing flights
class Airport{
 public:
  Airport();
  Airport(int ID,string name,string city,string country, string IATA,string ICAO,double latitude,double longitude);
  void addRoute(Route route);
  
  int ID_;
  string name_;
  string city_;
  string country_;
  string IATA_;
  string ICAO_;
  double latitude_;
  double longitude_;
  vector<Route> routes_;

};

class Graph{
 public:
  vector<Airport> airports_;
  vector<bool> visited_;

  int count;

  Graph();
  Graph(vector<Airport> airports);
  void DFT(Airport start);
  void DFTDis();
  std::pair<double,vector<Airport>> shortestPath(Airport air1, Airport air2);
  
  std::pair<double,vector<Airport>> landmarkPath(Airport air1, Airport air2,Airport air3);
  int minDistance(vector<double> dist,vector<Airport> spt);

  double haversine(double theta);
  
  double linDist(Airport u,Airport v);
  
};

