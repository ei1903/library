#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_1_A"

#include "../../template/template.cpp"
#include "../../graph/graph-template.cpp"

#include "../../structure/heap/radix-heap.cpp"

#include "../../graph/shortest-path/dijkstra-radix-heap.cpp"

int main() {
  int V, E, R;
  cin >> V >> E >> R;
  Graph< int > g(V);
  g.read(E, 0, true, true);
  for(auto &dist : dijkstra_radix_heap(g, R)) {
    if(dist == numeric_limits< int >::max()) cout << "INF\n";
    else cout << dist << "\n";
  }
}
