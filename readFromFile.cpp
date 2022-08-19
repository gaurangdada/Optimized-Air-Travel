#include "readFromFile.h"


vector<Route> readRoutes(string routes){
  
  const char * route = routes.c_str(); //converts string to const char
  vector<Route> v1; //vector returned
  std::vector<std::string> v2; 
  std::ifstream file;
  int val = 0;
  file.open(routes, std::ios::in); //read in file

  std::string line;
  //loop through all lines in file
  while(getline(file,line)){
    std::stringstream s(line); //stringstream of whole line
    //class members to be pushed
    string word;
    int airline = 0;
    int stops = 0;
    int source = 0;
    int dest = 0;
    bool code = false;
    vector<string> planes;

    //loop through each val in current line
    while (s >> std::ws) {
      string csvElement;
        val++;
	//check for commas and inside quotes, if so ignore
	if(s.peek() == '"'){
	  s >> std::quoted(csvElement);
	  string discard;
	  getline(s,discard,',');
	}
	else{
	  getline(s,csvElement,',');
	}
	//ignore null values
	if(csvElement == "\\N" || csvElement == "-"){
	  csvElement = "";
	}
	//airline IDX
	if(val == 2){
	  if(csvElement == ""){
	    csvElement = "-1";
	  }
	  //cout<<csvElement<<endl;
	  airline = std::stoi(csvElement);
	}
	//soure airport IDX
	else if(val == 4){
	  if(csvElement == ""){
	    csvElement = "-1";
	  }
	  source = std::stoi(csvElement);
	}
	//destination arirport IDX
	else if(val == 6){ 
	  if(csvElement == ""){
	    csvElement = "-1";
	  }
	  dest = std::stoi(csvElement);
	}
	
	else if(val == 7){
	  if(csvElement == "Y"){
	    code = true;
	  }
	  else{
	    code = false;
	  }
	}
	//number of stops usually 0
	else if(val == 8){
	  if(csvElement == ""){
	    csvElement = "-1";
	  }
	  stops = std::stoi(csvElement);
	}
	//generate a vector of planes 
	else if(val == 9){
	  string plane_name = "";
	  for(auto x : csvElement){
	    if(x == ' '){
	      planes.push_back(plane_name);
	      plane_name = "";
	    }
	    else{
	      plane_name += x;
	    }
	  }
	  
	    planes.push_back(plane_name);
    }
    }
    //push each route to vector
    v1.push_back(Route(airline,code,source,dest,stops,planes));
    planes.clear();
    val = 0;
  }
    file.close();
    return v1;
}
//read airports into a vector
vector<Airport> readAirports(string airports){
  
  const char * airport = airports.c_str();
  vector<Airport> v1; //return vector
  std::vector<std::string> v2;
  std::ifstream file;
  int val = 0;
  file.open(airport, std::ios::in); //open file

  std::string line;
  while(getline(file,line)){
    std::stringstream s(line);
    //values to be pushed
    string word;
    int ID = 0;
    string Name;
    string City;
    string Country;
    string IATA;
    string ICAO;
    double latitude;
    double longitude;
    
    //loop through each val in current line
    while (s >> std::ws) {
      string csvElement;
        val++;
	//check for commas in quoted strings, if so ignore them
	if(s.peek() == '"'){
	  s >> std::quoted(csvElement);
	  string discard;
	  getline(s,discard,',');
	}
	else{
	  getline(s,csvElement,',');
	}
	//ignore null elements
	if(csvElement == "\\N" || csvElement == "-"){
	  csvElement = "";
	}

	//assign elements based on position in string
	switch(val){
	  case 1:
	    ID = stoi(csvElement);
	  case 2:
	    Name = csvElement;
	  case 3:
	    City = csvElement;
	  case 4:
	    Country = csvElement;
	  case 5:
	    IATA = csvElement;
	  case 6:
	    ICAO = csvElement;
	  case 7:
	    latitude = std::atof(csvElement.c_str());
	  case 8:
	    longitude = std::atof(csvElement.c_str());
	}
	  

    }
    //push each airport to vector
    v1.push_back(Airport(ID,Name,City,Country,IATA,ICAO,latitude,longitude));
    val = 0;
  
  }
    file.close();
    return v1;
}

Graph makeGraph(string routesFile,string airportsFile,std::map<string,int> &nameMap){
  std::map<int, int> lineMap; //not used, causes seg fault if absent
  std::map<int, int> portMap; //first airport map, maps openFlight idx to vector idx

  
  vector<Route> routes;
  vector<Airport> airports;

  //read from files intially
  routes = readRoutes(routesFile);
  airports = readAirports(airportsFile);
  //map openFlight IDXs to vector indexs and change airport indexes
  for (unsigned i = 0; i < airports.size(); i++) {
    portMap.insert(std::pair<int, int>(airports[i].ID_, (int)i));
    airports[i].ID_ = i;
  }
  //change route airport indexes to airport indexes
  for (unsigned k = 0; k < routes.size(); k++) {
    routes[k].source_ = portMap.find(routes[k].source_)->second;
    routes[k].destination_ = portMap.find(routes[k].destination_)->second;
  }
  //add each route to its source airport
  for (unsigned i = 0; i < routes.size(); i++) {
    if (routes[i].source_ != -1 && routes[i].destination_ != -1) {
      airports[routes[i].source_].addRoute(routes[i]);
    }
  }
  //second airport map
  //used to eliminate unused airports
  std::map<int, int> portMap2;
  airports.erase(std::remove_if(airports.begin(), airports.end(),empty), airports.end());//removes all airports with no routes
  //maps intermediate indexes to new vector indexes
  for(unsigned i=0;i<airports.size();i++){
    portMap2.insert(std::pair<int,int>(airports[i].ID_,(int)i));
    airports[i].ID_ = i;
    airports[i].routes_.clear(); //empty route vector
    nameMap.insert(make_pair(airports[i].IATA_,(int)i)); //insert pair of name and index of airport
  }
  //update route indexes
  for (unsigned k = 0; k < routes.size(); k++) {
    routes[k].source_ = portMap2.find(routes[k].source_)->second;
    routes[k].destination_ = portMap2.find(routes[k].destination_)->second;
  }
  //place routes back in airports
  for (unsigned i = 0; i < routes.size(); i++) {
    if (routes[i].source_ != -1 && routes[i].destination_ != -1) {
      airports[routes[i].source_].addRoute(routes[i]);
    }
  }

  
  Graph g(airports); //create graph
  return g;
}
//helper function for deleting unused airports
bool empty(Airport air){
  return air.routes_.empty();
 }
