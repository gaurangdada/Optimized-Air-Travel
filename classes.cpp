#include "classes.h"

#include <iostream>
#include <climits>
#include <cmath>
#include <queue>

using std::cout;
using std::endl;


Route::Route(){
  line_=  0;
  stops_ = 0;
  source_ = 0;
  destination_ = 0;
  code_ = false;
  equipment_ = vector<string>();
}
Route::Route(int line,bool code,int source, int destination,int stops, vector<string> equipment){
  line_=  line;
  stops_ = stops;
  source_ = source;
  destination_ = destination;
  code_ = code;
  equipment_ = equipment;
}
Plane::Plane(){
  
  name_ = "";
  IATA_ = "";
  ICAO_ = "";
}
Plane::Plane(string name,string IATA,string ICAO){
  name_ = name;
  IATA_ = IATA;
  ICAO_ = ICAO;
}

Airline::Airline() {
  ID_ = 0;
  name_ = "";
  alias_ = "";
  IATA_ = "";
  ICAO_ = "";
  country_ = "";
  active_ = false;
}
Airline::Airline(int ID,string name,string alias, string IATA,string ICAO,string country,bool active){
  ID_ = ID;
  name_ = name;
  alias_ = alias;
  IATA_ = IATA;
  ICAO_ = ICAO;
  country_ = country;
  active_ = active;
}
Airport::Airport(){
  ID_ = -1;
  name_ = "";
  city_ = "";
  country_ = "";
  IATA_ = "";
  ICAO_ = "";
  latitude_ = 0;
  longitude_ = 0;
}
Airport::Airport(int ID,string name,string city,string country, string IATA,string ICAO,double latitude,double longitude){
  ID_ = ID;
  name_ = name;
  city_ = city;
  country_ = country;
  IATA_ = IATA;
  ICAO_ = ICAO;
  latitude_ = latitude;
  longitude_ = longitude;
  
}



void Airport::addRoute(Route route){
 routes_.push_back(route);
}



Graph::Graph(){
  airports_ = vector<Airport>();
  count = 0;
  for(unsigned i =0;i<airports_.size();i++){
    visited_.push_back(false);
  }
}

Graph::Graph(vector<Airport> airports){
  airports_ = airports;
  count = 0 ;
  for(unsigned i =0;i<airports_.size();i++){
    visited_.push_back(false);
  }
}

void Graph::DFT(Airport start){
  visited_[start.ID_] = true;
  for (unsigned i = 0; i < start.routes_.size(); i++) {
    //departing routes
      //map openFlights id to airport ID
      int idx = start.routes_[i].destination_;
      if(!visited_[idx]){
        count++;
        cout<<airports_[idx].name_<<endl;
        //recurse through all nodes
        DFT(airports_[idx]);
	}
         

    }
}
//disconnected implementation of Depth First Traversal
void Graph::DFTDis() {
  //loops through all elements 
  for(unsigned i =0;i<airports_.size();i++){
    //visits only the ones not already visited
    //this 
    if(visited_[i] == false){
      DFT(airports_[i]);
    }
  }
 
}
//Djikstra's algorithm using a priority queue
std::pair<double, vector<Airport>> Graph::shortestPath(Airport src,
                                                       Airport dest) {
  vector<double> dist; //current distance to all other nodes
  vector<Airport> previous; //for each node, stores the previous node in the traversal
  vector<bool> visited; //if a node is visited
  // priority queue used to store nodes, max head by default, so weights
  // are inverted
  std::priority_queue<std::pair<double, int>> q; 
  //loop through all airports
  for (unsigned i = 0; i < airports_.size(); i++) {
    if (airports_[i].ID_ != src.ID_) {
      dist.push_back(-INT_MAX); //push back -infinity
      previous.push_back(Airport()); //empty airport
    } else {
      dist.push_back(0);//push zero on source airport
      previous.push_back(Airport()); //push empty airport
    }
    visited.push_back(false); //clear visited
  }
  //pushes soure and its distance to queue 
  q.push(std::pair<double, int>(0, src.ID_));
  //until we've reached the destination
  while (q.top().second != dest.ID_) {
    //get top and pop it
    int u = q.top().second;
    q.pop();
    visited[u] = true; //visit node
    //for each edge in current node
    for (unsigned i = 0; i < airports_[u].routes_.size(); i++) {
      int dest_idx = airports_[u].routes_[i].destination_; //get destination index
      //get from current node to desination
      double currLength = linDist(airports_[u], airports_[dest_idx]);
      //if calculated distance is smaller than current calculated distance for destination and not visited
      if (dist[u] + currLength < -dist[dest_idx] && !visited[dest_idx]) {
	//calculate new best distance
        dist[dest_idx] = (dist[u] + currLength);
        previous[dest_idx] = airports_[u]; //pass current node into previous
	//push node into queue
	//queue will sort from closest to farthest
	q.push(std::pair<double, int>(-dist[dest_idx], dest_idx));
      }
    }
  }
  //start at destination
  Airport curr = dest;
  vector<Airport> path;
  //extract all airports in previous vector
  while (curr.ID_ != -1) {
    path.push_back(curr);
    curr = previous[curr.ID_];
  }
  //return shortest distance and path of airports
  return make_pair(-q.top().first, path);
}



//helper function for linDist
double Graph::haversine(double theta){
  return pow(sin(theta/2.0),2);
}

//great Circle distance between two airports
double Graph::linDist(Airport u,Airport v){
  double lat1 = u.latitude_;
  double lon1 = u.longitude_;
  double lat2 = v.latitude_;
  double lon2 = v.longitude_;
  const double rad = 0.01745329; // conversion degrees to radians
  double a = haversine(rad * (lat2 - lat1));
  double b = 1 - a - haversine(rad * (lat1 + lat2));
  double c = haversine(rad * (lon2 - lon1));
  const double r = 6371; // radius of the earth in KM
  return 2 * r * asin(sqrt(a + (b * c)));
}
