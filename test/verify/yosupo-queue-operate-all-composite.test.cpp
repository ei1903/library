#define PROBLEM "https://judge.yosupo.jp/problem/bernoulli_number"

#include "../../template/template.cpp"

#include "../../math/combinatorics/mod-int.cpp"

#include "../../structure/others/sliding-window-aggregation.cpp"

const int MOD = 998244353;
using mint = ModInt< MOD >;

int main() {
  int Q;
  cin >> Q;
  using pi = pair< mint, mint >;
  auto f = [](const pi &a, const pi &b) -> pi {
    return {a.first * b.first, a.second * b.first + b.second};
  };
  SlidingWindowAggregation< pi > swa(f);
  while(Q--) {
    int t;
    cin >> t;
    if(t == 0) {
      mint a, b;
      cin >> a >> b;
      swa.push(pi(a, b));
    } else if(t == 1) {
      swa.pop();
    } else {
      mint x;
      cin >> x;
      if(swa.empty()) {
        cout << x << "\n";
      } else {
        auto s = swa.fold_all();
        cout << s.first * x + s.second << "\n";
      }
    }
  }
}

