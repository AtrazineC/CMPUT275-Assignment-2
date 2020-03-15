//::    //::    //::    //::    //::    //::    //::    //::    //::    //::
    //::    //::    //::    //::    //::    //::    //::    //::    //::    //:
//::
    //::    By:                 Benjamin Kong -- 1573684
//::                            Lora Ma -------- 1570935
    //::
//::        CMPUT 275:          Winter 2020
    //::    Assignment 2:       Part 1
//::
    //::    //::    //::    //::    //::    //::    //::    //::    //::    //:
//::    //::    //::    //::    //::    //::    //::    //::    //::    //::

#include "wdigraph.h"
#include "dijkstra.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <stdlib.h>
#include <stack>
#include <string>

const char MAPNAME[] = "edmonton-roads-2.0.1.txt";

using namespace std;


struct Point {
long long lat; // latitude of the point
long long lon; // longitude of the point
};

long long manhattan(const Point& pt1, const Point& pt2) {
  long long manhattanDistance;
  manhattanDistance = abs(pt1.lat - pt2.lat) + abs(pt1.lon - pt2.lon);
  return manhattanDistance;
}

void readGraph(string filename, WDigraph& graph, unordered_map<int, Point>& points) {
/*
Read the Edmonton map data from the provided file
and load it into the given WDigraph object.
Store vertex coordinates in Point struct and map
each vertex to its corresponding Point struct.
PARAMETERS:
filename: name of the file describing a road network
graph: an instance of the weighted directed graph (WDigraph) class
points: a mapping between vertex identifiers and their coordinates
*/
/**
 * Creates a graph from the input file given (assuming it is formatted
 * correctly).
 *
 * @param filename The name of the file we want to open.
 *
 * @return The graph constructed from the data in the file.
 */
 /**
  * Creates a graph from the input file given (assuming it is formatted
  * correctly).
  *
  * @param filename The name of the file we want to open.
  *
  * @return The graph constructed from the data in the file.
  */

   ifstream file;
   file.open(filename);
   string current; // the current line being read

   while(!file.eof()) {
     getline(file, current);

     // Check if the line is a vertex ('V') or edge ('E').
     if (current[0] == 'V') {
       string j = current.substr(2, current.find(",", 2) - 2);
       graph.addVertex(stoi(j));
       int i = current.find(",", 2);
       double lon = stod(current.substr(current.find(",", i + 1) + 1));

       double lat = stod(current.substr(i + 1, current.find(",", i + 1) - i - 1));
       Point pt = {
          static_cast<long long>(lat * 100000),
          static_cast<long long>(lon * 100000)
      };
      points[stoi(j)] = pt;

     }
     else if (current[0] == 'E') {
       int comma1 = current.find(",", 2);
       int u = stoi(current.substr(2, comma1 - 2));
       int v = stoi(current.substr(comma1 + 1, current.find(",", comma1 + 1) - (comma1 + 1)));
       graph.addEdge(u, v, manhattan(points[u], points[v]));
     }
   }
   file.close();
 }



int main() {
    WDigraph graph;
    unordered_map<int, Point> pointMap;
    readGraph(MAPNAME, graph, pointMap);


    return 0;
}
