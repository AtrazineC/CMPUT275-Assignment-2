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

enum State {
    Waiting,
    Computing,
    Print
};

struct Point {
  long long lat; // latitude of the point
  long long lon; // longitude of the point
};

/**
 * Gets the cost between two points based on distance using Manhattan distance.
 *
 * @param pt1 The first point
 * @param pt2 The second point
 * 
 * @returns The cost between the two points
 */
long long manhattan(const Point& pt1, const Point& pt2) {
  long long manhattanDistance;
  manhattanDistance = abs(pt1.lat - pt2.lat) + abs(pt1.lon - pt2.lon);
  return manhattanDistance;
}

/**
 * Read the Edmonton map data from the provided file and load it into
 * the given WDigraph object.
 * Store vertex coordinates in Point struct and map
 * each vertex to its corresponding Point struct.
 *
 * @param filename The name of the file we want to open.
 * @param graph Instance of weighted directed graph class
 * @param points map between coordinates and vertex identifiers
 */
void readGraph(string filename, WDigraph& graph, unordered_map<int, Point>& points) {
  ifstream file;
  file.open(filename);
  string current; // the current line being read

  while(!file.eof()) {
    getline(file, current);

    // Check if the line is edge ('E').
    if (current[0] == 'E') {
      int comma1 = current.find(",", 2);
      int u = stoi(current.substr(2, comma1 - 2));
      int v = stoi(current.substr(comma1 + 1, current.find(",", comma1 + 1) - (comma1 + 1)));

      graph.addEdge(u, v, manhattan(points[u], points[v]));
    }
    //if line is vertex
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
 * @param map 
 */
stack<Point> calcPath(int start, int end, WDigraph &g, unordered_map<int, Point> &pointMap) {
  // shortest path from start to end stored here
  stack<Point> path; 

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


int main() {
  State currentState = Waiting;
  string action;
  Point beginning;
  Point end;

  WDigraph graph;
  unordered_map<int, Point> pointMap;
  readGraph(MAPNAME, graph, pointMap);

  while(true) {
    if (currentState == Waiting) {
      cin >> action;
      if (action == 'R') {
        cin >> beginning.lat;
        cin >> beginning.lon;
        cin >> end.lat;
        cin >> end.lon;
        currentState == Computing;
      }
    }
    else if (currentState == Computing) {
      auto vertex = getVertices(beginning, end, pointMap);
      path = getPath(vertex.first, vertex.second, graph, pointMap);
      currentState == Print;
    }
    else if (currentState == Print) {
      string acknowledge;
      cout << "N " << path.size() << endl;
      if (path.empty() == false) {
        cin >> acknowledge;
        while (acknowledge == "A" && path.empty() == false) {
          Point current = path.top();
          path.pop();
          cout << "W " << current.lat << " " << current.lon << endl;
          cin >> acknowledge;

        }
        cout << "E" << endl;
      }
      delete path;
      break;
    }
  }
  return 0;
}
