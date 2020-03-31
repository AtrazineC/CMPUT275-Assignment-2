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

#include "dijkstra.h"
#include "heap.h"
#include <unordered_map>

using namespace std;

typedef pair<int, int> vertex;

/**
 * Implementation of Dijkstra's algorithm to find paths with minimum
 * possible edge cost. The result is set in pass-by-reference parameter
 * `tree'.
 * 
 * @param graph The graph to search for a path
 * @param startVertex The initial node 
 * @param tree The spanning tree starting at the initial node
 */
void dijkstra(const WDigraph& graph, int startVertex, unordered_map<int, PIL>& tree) {
  // we use PIL tree to keep track of the spanning tree so far

  // empty heap to keep track of events
  BinaryHeap<vertex, long long> events;
  
  // start vertex burns at time 0
  vertex s = {startVertex, startVertex};
  events.insert(s, 0);

  // go through all the events
  while (events.size() > 0) {
    // pop the min item from the heap and get the data from it
    HeapItem<vertex, long long> data = events.min();

    vertex point = data.item;
    int u = point.first;
    int v = point.second;

    long long time = data.key;

    events.popMin();

    // if v hasn't been reached 
    if (tree.find(v) == tree.end()) {
      tree[v] = PIL(u, time); // add v to reached

      // iterate through v's neighboors
      for (auto it = graph.neighbours(v); it != graph.endIterator(v); it++) {
        // insert point into heap with new cost
        events.insert(vertex(v, *it), time + graph.getCost(v, *it));
      }
    }
  }
}
