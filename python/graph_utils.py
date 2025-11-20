import heapq
from collections import deque
from math_utils import INF64
from ds import DSU

def topo_sort(g):
    n = len(g)
    indeg = [0] * n
    for u in range(n):
        for v in g[u]:
            indeg[v] += 1
    q = deque([i for i in range(n) if indeg[i] == 0])
    res = []
    while q:
        u = q.popleft()
        res.append(u)
        for v in g[u]:
            indeg[v] -= 1
            if indeg[v] == 0:
                q.append(v)
    return res

def bfs_levels(g, src):
    n = len(g)
    dist = [-1] * n
    q = deque([src])
    dist[src] = 0
    while q:
        u = q.popleft()
        for v in g[u]:
            if dist[v] == -1:
                dist[v] = dist[u] + 1
                q.append(v)
    return dist

class LCA:
    def __init__(self, g, root=0):
        self.n = len(g)
        self.L = self.n.bit_length()
        self.up = [[-1] * self.n for _ in range(self.L)]
        self.depth = [0] * self.n
        stack = [(root, -1)]
        order = []
        while stack:
            u, p = stack.pop()
            order.append((u, p))
            self.up[0][u] = p
            for v in g[u]:
                if v == p:
                    continue
                self.depth[v] = self.depth[u] + 1
                stack.append((v, u))
        for k in range(1, self.L):
            for v in range(self.n):
                mid = self.up[k - 1][v]
                self.up[k][v] = -1 if mid == -1 else self.up[k - 1][mid]
    def lift(self, v, d):
        for k in range(self.L):
            if d & (1 << k):
                v = self.up[k][v]
        return v
    def query(self, a, b):
        if self.depth[a] < self.depth[b]:
            a, b = b, a
        a = self.lift(a, self.depth[a] - self.depth[b])
        if a == b:
            return a
        for k in range(self.L - 1, -1, -1):
            if self.up[k][a] != self.up[k][b]:
                a = self.up[k][a]
                b = self.up[k][b]
        return self.up[0][a]

def dijkstra(n, g, src):
    dist = [INF64] * n
    dist[src] = 0
    pq = [(0, src)]
    parent = [-1] * n
    while pq:
        d, u = heapq.heappop(pq)
        if d != dist[u]:
            continue
        for v, w in g[u]:
            nd = d + w
            if nd < dist[v]:
                dist[v] = nd
                parent[v] = u
                heapq.heappush(pq, (nd, v))
    return dist, parent

def bellman_ford(n, edges, src):
    dist = [INF64] * n
    dist[src] = 0
    for _ in range(n - 1):
        updated = False
        for u, v, w in edges:
            if dist[u] != INF64 and dist[v] > dist[u] + w:
                dist[v] = dist[u] + w
                updated = True
        if not updated:
            break
    for u, v, w in edges:
        if dist[u] != INF64 and dist[v] > dist[u] + w:
            return None
    return dist

def floyd_warshall(dist):
    n = len(dist)
    for k in range(n):
        for i in range(n):
            if dist[i][k] >= INF64:
                continue
            for j in range(n):
                if dist[k][j] >= INF64:
                    continue
                nd = dist[i][k] + dist[k][j]
                if nd < dist[i][j]:
                    dist[i][j] = nd
    return dist

def kruskal(n, edges):
    edges.sort(key=lambda x: x[2])
    dsu = DSU(n)
    total = 0
    chosen = []
    for u, v, w in edges:
        if dsu.unite(u, v):
            total += w
            chosen.append((u, v))
    return total, chosen

def bridges_and_arts(g):
    n = len(g)
    timer = 0
    tin = [-1] * n
    low = [-1] * n
    bridges = []
    arts = [False] * n
    def dfs(v, p):
        nonlocal timer
        tin[v] = low[v] = timer
        timer += 1
        children = 0
        for to in g[v]:
            if to == p:
                continue
            if tin[to] != -1:
                low[v] = min(low[v], tin[to])
            else:
                dfs(to, v)
                low[v] = min(low[v], low[to])
                if low[to] > tin[v]:
                    bridges.append((v, to))
                if low[to] >= tin[v] and p != -1:
                    arts[v] = True
                children += 1
        if p == -1 and children > 1:
            arts[v] = True
    for i in range(n):
        if tin[i] == -1:
            dfs(i, -1)
    return bridges, [i for i, x in enumerate(arts) if x]

class SCC:
    def __init__(self, g):
        self.n = len(g)
        self.g = g
        self.rg = [[] for _ in range(self.n)]
        for u in range(self.n):
            for v in g[u]:
                self.rg[v].append(u)
        self.vis = [False] * self.n
        self.order = []
        for i in range(self.n):
            if not self.vis[i]:
                self.dfs1(i)
        self.comp_id = [-1] * self.n
        self.comp = []
        for v in reversed(self.order):
            if self.comp_id[v] == -1:
                self.comp.append([])
                self.dfs2(v, len(self.comp) - 1)
    def dfs1(self, u):
        self.vis[u] = True
        for v in self.g[u]:
            if not self.vis[v]:
                self.dfs1(v)
        self.order.append(u)
    def dfs2(self, u, cid):
        self.comp_id[u] = cid
        self.comp[cid].append(u)
        for v in self.rg[u]:
            if self.comp_id[v] == -1:
                self.dfs2(v, cid)

class Dinic:
    def __init__(self, n):
        self.n = n
        self.g = [[] for _ in range(n)]
    def add_edge(self, u, v, cap):
        a = [v, cap, None]
        b = [u, 0, a]
        a[2] = b
        self.g[u].append(a)
        self.g[v].append(b)
    def bfs(self, s, t, level):
        for i in range(self.n):
            level[i] = -1
        q = deque([s])
        level[s] = 0
        while q:
            v = q.popleft()
            for to, cap, _ in self.g[v]:
                if cap > 0 and level[to] == -1:
                    level[to] = level[v] + 1
                    q.append(to)
        return level[t] != -1
    def dfs(self, v, t, f, level, it):
        if v == t or f == 0:
            return f
        while it[v] < len(self.g[v]):
            to, cap, rev = self.g[v][it[v]]
            if cap > 0 and level[to] == level[v] + 1:
                pushed = self.dfs(to, t, min(f, cap), level, it)
                if pushed:
                    self.g[v][it[v]][1] -= pushed
                    rev[1] += pushed
                    return pushed
            it[v] += 1
        return 0
    def max_flow(self, s, t):
        flow = 0
        level = [-1] * self.n
        while self.bfs(s, t, level):
            it = [0] * self.n
            while True:
                pushed = self.dfs(s, t, INF64, level, it)
                if not pushed:
                    break
                flow += pushed
        return flow
