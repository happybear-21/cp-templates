# Competitive Programming Templates

Repository of C++17 and Python 3.10 templates, split by topic and kept comment-free. Use the per-language `all` include/aggregate or pull in only the files you need.

## Layout
- `cpp/`: `base.hpp`, `math.hpp`, `ds.hpp`, `graph.hpp`, `strings.hpp`, `queries.hpp`, `dp.hpp`, `all.hpp`, `main.cpp`.
- `python/`: `math_utils.py`, `ds.py`, `graph_utils.py`, `string_utils.py`, `queries.py`, `dp.py`, `all.py`.

## Build/Run
- C++: `g++ -std=c++17 -O2 -pipe -static -s cpp/main.cpp -o main && ./main` (or include specific headers in your own file).
- Python: `python3 python/all.py` (or import specific modules; `all.py` binds `input = sys.stdin.readline`).

## Algorithm/DS Coverage
| Topic | C++17 Files | Python Files |
| --- | --- | --- |
| Core I/O/macros/constants/RNG | cpp/base.hpp | python/all.py (input binding) |
| Modular arithmetic, combinatorics, sieve, CRT, Miller-Rabin/Pollard Rho | cpp/math.hpp | python/math_utils.py |
| Fenwick (point/range), DSU, rollback DSU, segment trees (with lazy), sparse table, treap, sqrt-decomp, Li Chao, dynamic CHT | cpp/ds.hpp | python/ds.py (sans treap/CHT/sqrt/lazy) |
| Graphs: topo/BFS/DFS, LCA, shortest paths (Dijkstra/Bellman/Floyd), MST (Kruskal), bridges/articulation, SCC, Dinic flow | cpp/graph.hpp | python/graph_utils.py |
| Strings: Z, prefix-function, rolling hash, Manacher, suffix array + LCP | cpp/strings.hpp | python/string_utils.py |
| Offline queries: Mo, tree Mo | cpp/queries.hpp | python/queries.py |
| DP helper: divide-conquer DP optimizer | cpp/dp.hpp | python/dp.py |
