#define PROBLEM "https://judge.yosupo.jp/problem/bipartite_edge_coloring"

#include "../../template/template.cpp"

#include "../../structure/union-find/union-find.cpp"

#include "../../graph/flow/bipartite-flow.cpp"
#include "../../graph/others/eulerian-trail.cpp"
#include "../../graph/others/bipartite-graph-edge-coloring.cpp"

int main() {
  int L, R, M;
  cin >> L >> R >> M;
  BipariteGraphEdgeColoring ecbg;
  for(int i = 0; i < M; i++) {
    int a, b;
    cin >> a >> b;
    ecbg.add_edge(a, b);
  }
  auto res = ecbg.build();
  cout << res.size() << "\n";
  vector< int > color(M);
  for(int i = 0; i < res.size(); i++) {
    for(auto &j : res[i]) color[j] = i;
  }
  for(auto &c : color) cout << c << "\n";
}
