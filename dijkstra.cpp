#include "dijkstra.h"
#include "heap.h"
#include <unordered_map>

using namespace std;

typedef pair<int, int> vertex;

void dijkstra(const WDigraph& graph, int startVertex, unordered_map<int, PIL>& tree) {
    BinaryHeap<vertex, long long> events;

    pair<int, int> s = {startVertex, startVertex};
    events.insert(s, 0);

    while (events.size() > 0) {
        HeapItem<vertex, long long> data = events.min();
        events.popMin();

        vertex point = data.item;
        long long time = data.key;

        int u = point.first;
        int v = point.second;

        if (tree.find(v) == tree.end()) {
            tree[v] = PIL(u, time);
        }
    }

}