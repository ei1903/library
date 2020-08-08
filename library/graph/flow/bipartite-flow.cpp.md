---
layout: default
---

<!-- mathjax config similar to math.stackexchange -->
<script type="text/javascript" async
  src="https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.5/MathJax.js?config=TeX-MML-AM_CHTML">
</script>
<script type="text/x-mathjax-config">
  MathJax.Hub.Config({
    TeX: { equationNumbers: { autoNumber: "AMS" }},
    tex2jax: {
      inlineMath: [ ['$','$'] ],
      processEscapes: true
    },
    "HTML-CSS": { matchFontHeight: false },
    displayAlign: "left",
    displayIndent: "2em"
  });
</script>

<script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
<script src="https://cdn.jsdelivr.net/npm/jquery-balloon-js@1.1.2/jquery.balloon.min.js" integrity="sha256-ZEYs9VrgAeNuPvs15E39OsyOJaIkXEEt10fzxJ20+2I=" crossorigin="anonymous"></script>
<script type="text/javascript" src="../../../assets/js/copy-button.js"></script>
<link rel="stylesheet" href="../../../assets/css/copy-button.css" />


# :heavy_check_mark: Bipartite-Flow(二部グラフのフロー) <small>(graph/flow/bipartite-flow.cpp)</small>

<a href="../../../index.html">Back to top page</a>

* category: <a href="../../../index.html#2af6c4bb6ad7cfa010303133dc15971f">graph/flow</a>
* <a href="{{ site.github.repository_url }}/blob/master/graph/flow/bipartite-flow.cpp">View this file on GitHub</a>
    - Last commit date: 2020-08-09 02:21:24+09:00




## Verified with

* :heavy_check_mark: <a href="../../../verify/test/verify/aoj-0334.test.cpp.html">test/verify/aoj-0334.test.cpp</a>
* :heavy_check_mark: <a href="../../../verify/test/verify/yosupo-bipartitematching.test.cpp.html">test/verify/yosupo-bipartitematching.test.cpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
/**
 * @brief Bipartite-Flow(二部グラフのフロー)
 */
struct BipartiteFlow {
  size_t n, m, time_stamp;
  vector< vector< int > > graph;
  vector< int > match_l, match_r, dist, used, alive;

public:
  explicit BipartiteFlow(size_t n, size_t m) :
      n(n), m(m), graph(n), match_l(n, -1), match_r(m, -1), used(n), alive(n, 1), time_stamp(0) {}

  void add_edge(int u, int v) {
    graph[u].push_back(v);
  }

  vector< pair< int, int > > max_matching() {
    for(;;) {
      build_augment_path();
      ++time_stamp;
      int flow = 0;
      for(int i = 0; i < n; i++) {
        if(match_l[i] == -1) flow += find_min_dist_augment_path(i);
      }
      if(flow == 0) break;
    }
    vector< pair< int, int > > ret;
    for(int i = 0; i < n; i++) {
      if(match_l[i] >= 0) ret.emplace_back(i, match_l[i]);
    }
    return ret;
  }

  vector< pair< int, int > > lex_min_max_matching() {
    max_matching();
    for(auto &vs : graph) sort(begin(vs), end(vs));
    vector< pair< int, int > > es;
    for(int i = 0; i < n; i++) {
      if(match_l[i] == -1 || alive[i] == 0) {
        continue;
      }
      match_r[match_l[i]] = -1;
      match_l[i] = -1;
      ++time_stamp;
      find_augment_path(i);
      alive[i] = 0;
      es.emplace_back(i, match_l[i]);
    }
    return es;
  }

  vector< int > min_vertex_cover() {
    auto color = get_color();
    vector< int > ret;
    for(int i = 0; i < n; i++) {
      if(!color.first[i]) ret.emplace_back(i);
    }
    for(int i = 0; i < m; i++) {
      if(color.second[i]) ret.emplace_back(i + n);
    }
    return ret;
  }

  vector< int > max_independent_set() {
    auto color = get_color();
    vector< int > ret;
    for(int i = 0; i < n; i++) {
      if(color.first[i]) ret.emplace_back(i);
    }
    for(int i = 0; i < m; i++) {
      if(!color.second[i]) ret.emplace_back(i + n);
    }
    return ret;
  }

  vector< pair< int, int > > min_edge_cover() {
    auto es = max_matching();
    vector< int > need(m);
    for(int i = 0; i < m; i++) {
      if(match_r[i] == -1) {
        need[i] = 1;
      }
    }
    for(int i = 0; i < n; i++) {
      for(auto &to : graph[i]) {
        if(need[to] == 1) {
          need[to] = 0;
          es.emplace_back(i, to);
        }
      }
      if(match_l[i] >= 0) {
        continue;
      }
      if(graph[i].empty()) {
        return {};
      }
      es.emplace_back(i, graph[i][0]);
    }
    for(int i = 0; i < m; i++) {
      if(need[i] == 1) return {};
    }
    return es;
  }

private:
  void build_augment_path() {
    queue< int > que;
    dist.assign(graph.size(), -1);
    for(int i = 0; i < n; i++) {
      if(match_l[i] == -1) {
        que.emplace(i);
        dist[i] = 0;
      }
    }
    while(!que.empty()) {
      int a = que.front();
      que.pop();
      for(auto &b : graph[a]) {
        int c = match_r[b];
        if(c >= 0 && dist[c] == -1) {
          dist[c] = dist[a] + 1;
          que.emplace(c);
        }
      }
    }
  }

  bool find_min_dist_augment_path(int a) {
    used[a] = time_stamp;
    for(auto &b : graph[a]) {
      int c = match_r[b];
      if(c < 0 || (used[c] != time_stamp && dist[c] == dist[a] + 1 && find_min_dist_augment_path(c))) {
        match_r[b] = a;
        match_l[a] = b;
        return true;
      }
    }
    return false;
  }

  bool find_augment_path(int a) {
    used[a] = time_stamp;
    for(auto &b : graph[a]) {
      int c = match_r[b];
      if(c < 0 || (alive[c] == 1 && used[c] != time_stamp && find_augment_path(c))) {
        match_r[b] = a;
        match_l[a] = b;
        return true;
      }
    }
    return false;
  }

  pair< vector< int >, vector< int > > get_color() {
    max_matching();
    queue< int > que;
    vector< int > color_l(n), color_r(m);
    for(int i = 0; i < n; i++) {
      if(match_l[i] == -1) {
        color_l[i] = true;
        que.emplace(i);
      }
    }
    while(!que.empty()) {
      int a = que.front();
      que.pop();
      for(auto &b : graph[a]) {
        if(match_l[a] == b || color_r[b]) continue;
        color_r[b] = true;
        if(match_r[b] >= 0 && !color_l[match_r[b]]) {
          color_l[match_r[b]] = true;
          que.emplace(match_r[b]);
        }
      }
    }
    return {color_l, color_r};
  }
};

```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "graph/flow/bipartite-flow.cpp"
/**
 * @brief Bipartite-Flow(二部グラフのフロー)
 */
struct BipartiteFlow {
  size_t n, m, time_stamp;
  vector< vector< int > > graph;
  vector< int > match_l, match_r, dist, used, alive;

public:
  explicit BipartiteFlow(size_t n, size_t m) :
      n(n), m(m), graph(n), match_l(n, -1), match_r(m, -1), used(n), alive(n, 1), time_stamp(0) {}

  void add_edge(int u, int v) {
    graph[u].push_back(v);
  }

  vector< pair< int, int > > max_matching() {
    for(;;) {
      build_augment_path();
      ++time_stamp;
      int flow = 0;
      for(int i = 0; i < n; i++) {
        if(match_l[i] == -1) flow += find_min_dist_augment_path(i);
      }
      if(flow == 0) break;
    }
    vector< pair< int, int > > ret;
    for(int i = 0; i < n; i++) {
      if(match_l[i] >= 0) ret.emplace_back(i, match_l[i]);
    }
    return ret;
  }

  vector< pair< int, int > > lex_min_max_matching() {
    max_matching();
    for(auto &vs : graph) sort(begin(vs), end(vs));
    vector< pair< int, int > > es;
    for(int i = 0; i < n; i++) {
      if(match_l[i] == -1 || alive[i] == 0) {
        continue;
      }
      match_r[match_l[i]] = -1;
      match_l[i] = -1;
      ++time_stamp;
      find_augment_path(i);
      alive[i] = 0;
      es.emplace_back(i, match_l[i]);
    }
    return es;
  }

  vector< int > min_vertex_cover() {
    auto color = get_color();
    vector< int > ret;
    for(int i = 0; i < n; i++) {
      if(!color.first[i]) ret.emplace_back(i);
    }
    for(int i = 0; i < m; i++) {
      if(color.second[i]) ret.emplace_back(i + n);
    }
    return ret;
  }

  vector< int > max_independent_set() {
    auto color = get_color();
    vector< int > ret;
    for(int i = 0; i < n; i++) {
      if(color.first[i]) ret.emplace_back(i);
    }
    for(int i = 0; i < m; i++) {
      if(!color.second[i]) ret.emplace_back(i + n);
    }
    return ret;
  }

  vector< pair< int, int > > min_edge_cover() {
    auto es = max_matching();
    vector< int > need(m);
    for(int i = 0; i < m; i++) {
      if(match_r[i] == -1) {
        need[i] = 1;
      }
    }
    for(int i = 0; i < n; i++) {
      for(auto &to : graph[i]) {
        if(need[to] == 1) {
          need[to] = 0;
          es.emplace_back(i, to);
        }
      }
      if(match_l[i] >= 0) {
        continue;
      }
      if(graph[i].empty()) {
        return {};
      }
      es.emplace_back(i, graph[i][0]);
    }
    for(int i = 0; i < m; i++) {
      if(need[i] == 1) return {};
    }
    return es;
  }

private:
  void build_augment_path() {
    queue< int > que;
    dist.assign(graph.size(), -1);
    for(int i = 0; i < n; i++) {
      if(match_l[i] == -1) {
        que.emplace(i);
        dist[i] = 0;
      }
    }
    while(!que.empty()) {
      int a = que.front();
      que.pop();
      for(auto &b : graph[a]) {
        int c = match_r[b];
        if(c >= 0 && dist[c] == -1) {
          dist[c] = dist[a] + 1;
          que.emplace(c);
        }
      }
    }
  }

  bool find_min_dist_augment_path(int a) {
    used[a] = time_stamp;
    for(auto &b : graph[a]) {
      int c = match_r[b];
      if(c < 0 || (used[c] != time_stamp && dist[c] == dist[a] + 1 && find_min_dist_augment_path(c))) {
        match_r[b] = a;
        match_l[a] = b;
        return true;
      }
    }
    return false;
  }

  bool find_augment_path(int a) {
    used[a] = time_stamp;
    for(auto &b : graph[a]) {
      int c = match_r[b];
      if(c < 0 || (alive[c] == 1 && used[c] != time_stamp && find_augment_path(c))) {
        match_r[b] = a;
        match_l[a] = b;
        return true;
      }
    }
    return false;
  }

  pair< vector< int >, vector< int > > get_color() {
    max_matching();
    queue< int > que;
    vector< int > color_l(n), color_r(m);
    for(int i = 0; i < n; i++) {
      if(match_l[i] == -1) {
        color_l[i] = true;
        que.emplace(i);
      }
    }
    while(!que.empty()) {
      int a = que.front();
      que.pop();
      for(auto &b : graph[a]) {
        if(match_l[a] == b || color_r[b]) continue;
        color_r[b] = true;
        if(match_r[b] >= 0 && !color_l[match_r[b]]) {
          color_l[match_r[b]] = true;
          que.emplace(match_r[b]);
        }
      }
    }
    return {color_l, color_r};
  }
};

```
{% endraw %}

<a href="../../../index.html">Back to top page</a>
