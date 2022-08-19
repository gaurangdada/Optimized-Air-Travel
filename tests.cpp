#include "../cs225/catch/catch.hpp"
#include "../cs225/PNG.h"
#include "../cs225/HSLAPixel.h"
#include <map>
#include "../classes/classes.h"
#include "../readFromFile.h"
#include <string>
#include <vector>
#include <fstream>

using std::string;

std::map<string,int> nameMap;
 Graph g = makeGraph("data/routes.dat","data/airports.dat",nameMap);

TEST_CASE("Check File processing","[weight=2][part=1]"){
  vector<Route> routes = readRoutes("data/routes.dat");
  vector<Airport> airports = readAirports("data/airports.dat");
  string line;
  fstream file("data/routes.dat");
  
  int routesCount =0;
  while(!file.eof()){
    getline(file,line);
    routesCount++;
  }
  file.close();
  fstream file2("data/airports.dat");
  int airportCount = 0;
  while(!file2.eof()){
    getline(file2,line);
    airportCount++;
  }
  file2.close();
  //check that readRoutes and readAirports import all lines of files
  REQUIRE(airports.size() == airportCount);
  REQUIRE(routes.size() == routesCount);
}
TEST_CASE("Check post-processing","[weight=2][part=1]"){
  bool allHaveRoutes = true;
  bool allHaveNames = true;
  bool correctRouteIDX = true;
  for(unsigned i=0;i<g.airports_.size();i++){
    if(g.airports_[i].routes_.size() == 0){
      allHaveRoutes = false;
    }
    if(g.airports_[i].name_ == ""){
      allHaveNames = false;
    }

  }

  REQUIRE(allHaveRoutes);
  REQUIRE(allHaveNames);
}
//checks that generated nameMap correctly maps IDs to IATA values for each airport
TEST_CASE("Check nameMap","[weight=2][part=1]"){
  for(unsigned i=0;i<g.airports_.size();i++){
    REQUIRE(g.airports_[nameMap.find(g.airports_[i].IATA_)->second].IATA_ == g.airports_[i].IATA_);
  }
}




//checks if linDistance function returns correct value
TEST_CASE("Check distance","[weight=2][part=1]"){
  //distance from New York to Los angeles in Km
  const double NY_LA = 3973;
  double NY_LA_computed = g.linDist(g.airports_[nameMap.find("JFK")->second],g.airports_[nameMap.find("LAX")->second]);
  
  double LA_NY_computed = g.linDist(g.airports_[nameMap.find("LAX")->second],g.airports_[nameMap.find("JFK")->second]);
  //check that computed distance is within 0.1% of true value
  bool checkNL = .999*NY_LA <= NY_LA_computed && NY_LA_computed <= 1.001*NY_LA;
  REQUIRE(checkNL);
  //check  that distance works in both directions
  bool checkLN = LA_NY_computed == NY_LA_computed;

  REQUIRE(checkLN);
}


TEST_CASE("Check if Dijkstra's works for direct flights") {
  
    std::map<string, int> nameMap;
    Graph graph = makeGraph("data/test-routes.dat","data/test-airports.dat",nameMap);
     
    std::pair<double,vector<Airport>> output = graph.shortestPath(g.airports_[0], g.airports_[2]);
     vector<Airport> path = output.second;
     double pathDist = output.first;
     bool check = 0.99*124.48 <=pathDist && pathDist <= 1.01*124.48;
     REQUIRE(graph.airports_.size() == 3);
     REQUIRE(check);
 }

TEST_CASE("Check if Dijkstra's works for not direct flights") {
     
    std::map<string, int> nameMap;
    Graph graph = makeGraph("data/test1-routes.dat","data/test1-airports.dat",nameMap);
    
    std::pair<double,vector<Airport>> output = graph.shortestPath(g.airports_[0], g.airports_[5]);
     vector<Airport> path = output.second;
     double pathDist = output.first;
     bool check = 0.99*337.022 <=pathDist && pathDist <= 1.01*337.022;
     REQUIRE(graph.airports_.size() == 6);
     REQUIRE(check); 
}

//traversal

TEST_CASE("Check if traversal is working") {
  
    std::map<string, int> nameMap;
    Graph graph = makeGraph("data/test1-routes.dat","data/test1-airports.dat",nameMap);
    graph.DFTDis();
    for (unsigned i = 0; i < graph.airports_.size(); i++) {
      REQUIRE(graph.visited_[i] == true);
    }
}

TEST_CASE("Check if traversal is working for unconnected components") {
    
    std::map<string, int> nameMap;
    Graph graph = makeGraph("data/test1-routes.dat","data/test1-airports.dat",nameMap);
    graph.DFTDis();
    for (unsigned i = 0; i < graph.airports_.size(); i++) {
        REQUIRE(graph.visited_[i] == true);
    }
}
