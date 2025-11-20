# C++ Template Cheatsheet

Each component is in its own header (no inline comments). Include what you need or `all.hpp` for everything. `main.cpp` is a stub wired with `FAST_IO`.

- `base.hpp`: standard includes, `FAST_IO`, `ALL/SZ/REP/FOR/FORD`, types `ll/ull/pii/pll`, constants `INF32/INF64/MOD/MOD2`, RNG.
- `math.hpp`: `mod_pow`, `mod_inv`, `lcm_ll`, `Comb` (nCr), `LinearSieve`, 64-bit `is_probable_prime` + `pollard_rho` + `factor_rec`, `crt_pair`.
- `ds.hpp`: `Fenwick`, `FenwickRange`, `DSU`, `RollbackDSU`, `SegmentTree`, `LazySegTree`, `SparseTable`, `Treap`, `SqrtDecomp`, `LiChao`, `LineContainer` (max CHT).
- `graph.hpp`: `topo_sort`, `bfs_levels`, `dfs_rec`, `LCA`, `dijkstra`, `bellman_ford`, `floyd_warshall`, `kruskal`, `BridgeFinder`, `SCC`, `Dinic`.
- `strings.hpp`: `z_function`, `prefix_function`, `RollingHash`, `manacher`, `suffix_array`, `lcp_array`.
- `queries.hpp`: `Mo` (array) and `TreeMo` (Euler-tour toggle).
- `dp.hpp`: `divide_conquer_dp` for monotone-opt D&C DP.
- `all.hpp`: convenience aggregate include.
- `main.cpp`: sample entry with `FAST_IO`; include headers you want.
