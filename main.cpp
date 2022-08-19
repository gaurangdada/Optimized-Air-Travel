#include "readFromFile.h"
#include "classes/classes.h"
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
#include "Animation.h"
#include <tuple>
using namespace cs225;




void placeDot(PNG *img,double latitude,double longitude,int color){
  
  int centerx = img->width()/2; //center pixels on each axis
  int centery = img->height()/2;
  const double rad = M_PI/(double)180; //conversion between degrees and radians
  //x axis scales linearly, with degree, line below centers 0 longitude at 1021
  double x = (double)centerx*((double)180+longitude)/(double)180 -4;
  //Formula for converting latitude to a y coordiante
  double y =  img->height()/2+(centery/M_PI)*log(1/cos((-latitude)*rad) + tan((-latitude)*rad));
  //create a 5x5 grid around location of airport 
  for(int i =-2; i< 2;i++){
    for(int j = -2;j<2;j++){
      cs225::HSLAPixel & pixel1 = img->getPixel((int)x+i,(int)y+j);
      pixel1.h = color;
    }
 }
}
void drawLine(PNG *img,Airport air1,Airport air2,int color){

  //store input members for ease of access
  double latitude1 = air1.latitude_;
  double longitude1 = air1.longitude_;
  double latitude2 = air2.latitude_;
  double longitude2 = air2.longitude_;
  //find eqution of line y = mx +b for two points
   double m = (latitude2 - latitude1)/(longitude2-longitude1);
   double b = latitude1 - m*longitude1;
  
   double currlat = latitude1;
   double currlon = longitude1;
   int left_right; //if line is going left to right or right to left

   
   if(longitude1>longitude2){
     left_right = -1;
   }
   else{
     left_right = 1;
   }

   //check if current pixel being drawn is within a range of target.
   //if value is too small, line witll overshoot
   while(abs(currlat - latitude2) > 0.1 && abs(currlon-longitude2) > 0.1){
     placeDot(img,currlat,currlon,color);
     currlon += left_right*0.1; //move by a small amount either left or right
     currlat = m*currlon + b; //recalculate latitude

}
   //bound conditions shown in inverse colors for visibility
   placeDot(img,latitude1,longitude1,255-color);
   placeDot(img,latitude2,longitude2,255-color);
}

int main(int argc, char **argv) {
  std::map<string, int> nameMap;
  std::pair<double, vector<Airport>> path;
  string out;
  Graph g;
  // chekc number of inputs, if Djikstra's inputs specified
  if (argc >= 3) {
    // if data files specified
    if (argc >= 5) {
      g = makeGraph(argv[3], argv[4], nameMap);
    }
    //default file locations
    else {
      g = makeGraph("data/routes.dat", "data/airports.dat", nameMap);
    }
    //calculate shortest path with inputs
    path = g.shortestPath(g.airports_[nameMap.find(argv[1])->second],
                          g.airports_[nameMap.find(argv[2])->second]);
    string name = "_";
    string name1 = ".png";
    out = argv[1] + name + argv[2] + name1; //custom file name
  }
  
  else {
    g = makeGraph("data/routes.dat", "data/airports.dat", nameMap);
    path = g.shortestPath(g.airports_[nameMap.find("JFK")->second],
                          g.airports_[nameMap.find("LAX")->second]);
    out = "JFK_LAX.png";
  }
  g.DFT(g.airports_[0]); //Depth first search starting at 
  
  cout<<path.first<<endl; //print calculated distance
  PNG *img = new PNG(); //Djikstra's image
  PNG *portsImg = new PNG(); //image of all airports with routes
  img->readFromFile("world.png");
  portsImg->readFromFile("world.png");
  //draws all lines in path
  for (unsigned i = 0; i < path.second.size() - 1; i++) {
    drawLine(img, path.second[i], path.second[i + 1], 255); 
  }
  //draw all airports with routes
  for (unsigned i = 0; i < g.airports_.size(); i++) {
    placeDot(portsImg, g.airports_[i].latitude_, g.airports_[i].longitude_, 0);
  }
  //write images out and delete pointers
  img->writeToFile(out); 
  portsImg->writeToFile("airports.png");
  delete img;
  delete portsImg;

  return 0;
}
