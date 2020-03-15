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

using namespace std;

const char MAPNAME[] = "edmonton-roads-2.0.1.txt";

enum State {
    Waiting,
    CMPUTing, // :)))
    Print 
};

struct Point {
  long long lat; 
  long long lon; 
};

typedef stack<Point> sequence;
typedef pair<int, int> vertex;

/**
 * Gets the cost between two points based on distance using Manhattan distance.
 *
 * @param pt1 The first point
 * @param pt2 The second point
 * 
 * @returns The cost between the two points
 */
long long manhattan(const Point& pt1, const Point& pt2) {
  return abs(pt1.lat - pt2.lat) + abs(pt1.lon - pt2.lon);
}

/**
 * Read the Edmonton map data from the provided file and load it into
 * the given WDigraph object.
 * Store vertex coordinates in Point struct and map
 * each vertex to its corresponding Point struct.
 *
 * @param filename The name of the file we want to open.
 * @param graph Instance of weighted directed graph class
 * @param points Map between coordinates and vertex identifiers
 */
void readGraph(string filename, WDigraph& graph, unordered_map<int, Point>& points) {
  ifstream file;
  file.open(filename);
  string current; // the current line being read

  while(!file.eof()) {
    getline(file, current);

    // Check if the line is edge (`E').
    if (current[0] == 'E') {
      int comma1 = current.find(",", 2);
      int u = stoi(current.substr(2, comma1 - 2));
      int v = stoi(current.substr(comma1 + 1, current.find(",", comma1 + 1) - (comma1 + 1)));

      graph.addEdge(u, v, manhattan(points[u], points[v]));
    }
    // Check if line is vertex (`V')
    else if (current[0] == 'V') {
      string j = current.substr(2, current.find(",", 2) - 2);
      graph.addVertex(stoi(j));
      int i = current.find(",", 2);

      double lon = stod(current.substr(current.find(",", i + 1) + 1));
      double lat = stod(current.substr(i + 1, current.find(",", i + 1) - i - 1));

      //convert to long long variable
      Point pt = {
        static_cast<long long>(lat * 100000),
        static_cast<long long>(lon * 100000)
      };

      points[stoi(j)] = pt;
    }
  }

  file.close();
}

/**
 * Finds the shortest path between two points in a given graph using Dijkstra's.
 *
 * @param start The starting point
 * @param end The ending point
 * @param g The weighted, directed graph to be considered
 * @param pointMap Map between coordinates and vertex identifiers
 */
sequence calcPath(int start, int end, WDigraph &g, unordered_map<int, Point> &pointMap) {
  // shortest path from start to end stored here
  sequence path; 

  // stores search tree
  unordered_map<int, PIL> tree; 

  // generate search tree starting from the start node
  dijkstra(g, start, tree); 

  // trace the path, starting at the end node
  int current = end;

  // if the tree contains the end point, then a path exists that we will find.
  if (tree.find(current) != tree.end()) {
    // trace the path until it reachs the start, adding each point to the stack.
    while (current != start) {
      PIL next = tree[current];
      path.push(pointMap[current]);
      current = next.first; 
    }

    // add the start point to the top of the stack
    path.push(pointMap[start]);
  }

  return path;
}

/**
 * Finds the closest vertex to the two given points.
 *
 * @param a The starting point
 * @param b The ending point
 * @param pointMap Map between coordinates and vertex identifiers
 * 
 * @returns Vertex that is closest to the two points given.
 */
vertex getClosestVertex(Point a, Point b, unordered_map<int, Point> &pointMap) {
  vertex current = {pointMap.begin()->first, pointMap.begin()->first};

  // iterate through all the points 
  for (auto it = pointMap.begin(); it != pointMap.end(); it++) {
    // if the point is closer, update current accordingly
    if (manhattan(it->second, a) < manhattan(pointMap[current.first], a)) {
      current.first = it->first;
    }

    if (manhattan(it->second, b) < manhattan(pointMap[current.second], b)) {
      current.second = it->first;
    }   
  }

  return current;
}

/**
 * Main function of program.
 */
int main() {
  State state = Waiting;
  
  Point start;
  Point end;

  sequence path;

  WDigraph graph;
  unordered_map<int, Point> pointMap;
  readGraph(MAPNAME, graph, pointMap);

  while(true) {
    // State: waiting for input.
    if (state == Waiting) {
      char action; cin >> action;

      if (action == 'R') {
        cin >> start.lat;
        cin >> start.lon;
        cin >> end.lat;
        cin >> end.lon;

        state = CMPUTing;
      }
    }
    // State: calculating path.
    else if (state == CMPUTing) {
      vertex v = getClosestVertex(start, end, pointMap);
      path = calcPath(v.first, v.second, graph, pointMap);

      state = Print;
    }
    // State: output result.
    else if (state == Print) {
      string acknowledge;
      cout << "N " << path.size() << endl; // Start of output

      // if the path is not empty
      if (path.empty() == false) {
        Point current;
        cin >> acknowledge;

        // iterate through each point in the path stack
        while (acknowledge == "A" && path.empty() == false) {
          current = path.top();
          path.pop();
          cout << "W " << current.lat << " " << current.lon << endl;
          cin >> acknowledge;
        }

        cout << "E" << endl; // End of output
      }

      break; // Done. Break out of loop.
    }
  }

  return 0;
}
