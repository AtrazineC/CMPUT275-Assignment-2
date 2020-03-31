//::    //::    //::    //::    //::    //::    //::    //::    //::    //::
    //::    //::    //::    //::    //::    //::    //::    //::    //::    //:
//::
    //::    By:                 Benjamin Kong -- 1573684
//::                            Lora Ma -------- 1570935
    //::
//::        CMPUT 275:          Winter 2020
    //::    Assignment 2:       Part 2
//::
    //::    //::    //::    //::    //::    //::    //::    //::    //::    //:
//::    //::    //::    //::    //::    //::    //::    //::    //::    //::

#include <iostream>
#include <cmath>
#include <fstream>
#include <stdlib.h>
#include <stack>
#include <queue>
#include <string>
#include <algorithm>
#include "wdigraph.h"
#include "dijkstra.h"
#include "serialport.h"

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

  // check if start and end points are the same
  if (start == end) {
    path.push(pointMap[end]);
    return path;
  }

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
 * Reads a line of input from the Arduino and writes the commands into a queue
 * that is passed in by reference.
 *
 * @param line The line of input read from the Arduino (client)
 * @param items A pass by reference queue that stores the inputs once they
 *              have been parsed
 */
void interpretLine(string line, queue<string>& items) {
  auto current_start = line.begin();
  auto current_end = find(current_start, line.end(), ' ');

  while (current_end != line.end()) {
    items.push(string(current_start, current_end));
    current_start = current_end + 1; // move to next item
    current_end = find(current_start, line.end(), ' ');
  }

  items.push(string(current_start, current_end));
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

  SerialPort client; // assumes arduino connected to /dev/ttyACM0
  int max_timeout = 1000; // 1000 ms timeout

  while(true) {
    // State: waiting for input.
    if (state == Waiting) {
      // Read in the line from the client and interpret it into the start and 
      // end points
      string line = client.readline(max_timeout);
      queue<string> Q;

      interpretLine(line, Q);

      string action = Q.front(); Q.pop();
      
      if (action == "R") {
        // stoll converts a string to long long
        start.lat = stoll(Q.front()); Q.pop(); 
        start.lon = stoll(Q.front()); Q.pop();
        end.lat = stoll(Q.front()); Q.pop(); 
        end.lon = stoll(Q.front()); Q.pop();

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
      client.writeline("N ");
      client.writeline(to_string(path.size()));
      client.writeline("\n");

      // if the path is not empty
      if (!path.empty()) {
        Point current;
        acknowledge = client.readline(max_timeout);

        // iterate through each point in the path stack
        while (!path.empty()) {
          if (acknowledge[0] == 'A') {
            current = path.top();
            path.pop();

            client.writeline("W ");
            client.writeline(to_string(current.lat));
            client.writeline(" ");
            client.writeline(to_string(current.lon));
            client.writeline("\n");
          }
          
          acknowledge = client.readline(max_timeout);
        }

        client.writeline("E\n"); // End of output
      }

      state = Waiting;
    }
  }

  return 0;
}
