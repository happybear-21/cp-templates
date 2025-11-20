# Python Template Cheatsheet

Modules are split by topic (no inline comments). Use `all.py` to import everything; `input` is set to `sys.stdin.readline` there.

- `math_utils.py`: `INF64/MOD/MOD2`, `mod_pow`, `mod_inv`, `lcm`, `Comb`, `linear_sieve`, `is_probable_prime`, `pollard_rho`, `factor`, `crt_pair`, helpers `mul_mod`, `mod_pow_u64`.
- `ds.py`: `Fenwick`, `FenwickRange`, `DSU`, `RollbackDSU`, `SegmentTree`, `SparseTable`.
- `graph_utils.py`: `topo_sort`, `bfs_levels`, `LCA`, `dijkstra`, `bellman_ford`, `floyd_warshall`, `kruskal`, `bridges_and_arts`, `SCC`, `Dinic`.
- `string_utils.py`: `z_function`, `prefix_function`, `RollingHash`, `manacher`, `suffix_array`, `lcp_array`.
- `queries.py`: `mo_process` (array), `tree_mo` (Euler toggle style).
- `dp.py`: `divide_conquer_dp` for monotone-optimize D&C DP.
- `all.py`: convenience import of everything plus fast input binding.
