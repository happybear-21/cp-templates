#ifndef CP_GRAPH_HPP
#define CP_GRAPH_HPP

#include "base.hpp"
#include "ds.hpp"

inline vector<int> topo_sort(const vector<vector<int>>& g) {
    int n = SZ(g);
    vector<int> indeg(n), res;
    for (int u = 0; u < n; u++) for (int v : g[u]) indeg[v]++;
    queue<int> q;
    for (int i = 0; i < n; i++) if (!indeg[i]) q.push(i);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        res.push_back(u);
        for (int v : g[u]) if (--indeg[v] == 0) q.push(v);
    }
    return res;
}

inline vector<int> bfs_levels(const vector<vector<int>>& g, int src) {
    int n = SZ(g);
    vector<int> dist(n, -1);
    queue<int> q;
    dist[src] = 0; q.push(src);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) if (dist[v] == -1) {
            dist[v] = dist[u] + 1;
            q.push(v);
        }
    }
    return dist;
}

inline void dfs_rec(int u, const vector<vector<int>>& g, vector<int>& vis, vector<int>& order) {
    vis[u] = 1;
    for (int v : g[u]) if (!vis[v]) dfs_rec(v, g, vis, order);
    order.push_back(u);
}

struct LCA {
    int n, L;
    vector<vector<int>> up;
    vector<int> depth;
    LCA(const vector<vector<int>>& g, int root = 0) {
        n = SZ(g);
        L = 1;
        while ((1 << L) <= n) ++L;
        up.assign(L, vector<int>(n, -1));
        depth.assign(n, 0);
        function<void(int,int)> dfs = [&](int u, int p) {
            up[0][u] = p;
            for (int v : g[u]) if (v != p) {
                depth[v] = depth[u] + 1;
                dfs(v, u);
            }
        };
        dfs(root, -1);
        for (int k = 1; k < L; k++) {
            for (int v = 0; v < n; v++) {
                int mid = up[k - 1][v];
                up[k][v] = mid == -1 ? -1 : up[k - 1][mid];
            }
        }
    }
    int lift(int v, int d) const {
        for (int k = 0; k < L; k++) if (d & (1 << k)) v = up[k][v];
        return v;
    }
    int query(int a, int b) const {
        if (depth[a] < depth[b]) swap(a, b);
        a = lift(a, depth[a] - depth[b]);
        if (a == b) return a;
        for (int k = L - 1; k >= 0; k--) if (up[k][a] != up[k][b]) {
            a = up[k][a];
            b = up[k][b];
        }
        return up[0][a];
    }
};

inline void dijkstra(int n, const vector<vector<pair<int,int>>>& g, int src, vector<ll>& dist, vector<int>* parent = nullptr) {
    dist.assign(n, INF64);
    if (parent) parent->assign(n, -1);
    using P = pair<ll, int>;
    priority_queue<P, vector<P>, greater<P>> pq;
    dist[src] = 0; pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dist[u]) continue;
        for (auto [v, w] : g[u]) if (dist[v] > d + w) {
            dist[v] = d + w;
            if (parent) (*parent)[v] = u;
            pq.push({dist[v], v});
        }
    }
}

inline bool bellman_ford(int n, const vector<tuple<int,int,int>>& edges, int src, vector<ll>& dist) {
    dist.assign(n, INF64);
    dist[src] = 0;
    for (int i = 0; i < n - 1; i++) {
        bool any = false;
        for (auto [u, v, w] : edges) {
            if (dist[u] == INF64) continue;
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                any = true;
            }
        }
        if (!any) break;
    }
    for (auto [u, v, w] : edges) if (dist[u] != INF64 && dist[v] > dist[u] + w) return false;
    return true;
}

inline vector<vector<ll>> floyd_warshall(vector<vector<ll>> dist) {
    int n = SZ(dist);
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) if (dist[i][k] < INF64) {
            for (int j = 0; j < n; j++) if (dist[k][j] < INF64) {
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }
    return dist;
}

inline pair<ll, vector<pair<int,int>>> kruskal(int n, vector<tuple<int,int,int>> edges) {
    sort(ALL(edges), [](auto& a, auto& b){ return get<2>(a) < get<2>(b); });
    DSU dsu(n);
    ll total = 0;
    vector<pair<int,int>> chosen;
    for (auto [u, v, w] : edges) {
        if (dsu.unite(u, v)) {
            total += w;
            chosen.push_back({u, v});
        }
    }
    return {total, chosen};
}

struct BridgeFinder {
    int n, timer;
    vector<vector<int>> g;
    vector<int> tin, low, is_art;
    vector<pair<int,int>> bridges;
    BridgeFinder(const vector<vector<int>>& g) : n(SZ(g)), g(g), timer(0), tin(n, -1), low(n, -1), is_art(n, 0) {
        for (int i = 0; i < n; i++) if (tin[i] == -1) dfs(i, -1);
    }
    void dfs(int v, int p) {
        tin[v] = low[v] = timer++;
        int child = 0;
        for (int to : g[v]) if (to != p) {
            if (tin[to] != -1) low[v] = min(low[v], tin[to]);
            else {
                dfs(to, v);
                low[v] = min(low[v], low[to]);
                if (low[to] > tin[v]) bridges.push_back({v, to});
                if (low[to] >= tin[v] && p != -1) is_art[v] = 1;
                ++child;
            }
        }
        if (p == -1 && child > 1) is_art[v] = 1;
    }
};

struct SCC {
    int n, idx;
    vector<vector<int>> g, rg, comp;
    vector<int> order, comp_id, vis;
    SCC(const vector<vector<int>>& g) : n(SZ(g)), g(g), rg(n), idx(0), comp_id(n, -1), vis(n, 0) {
        for (int u = 0; u < n; u++) for (int v : g[u]) rg[v].push_back(u);
        for (int i = 0; i < n; i++) if (!vis[i]) dfs1(i);
        reverse(ALL(order));
        for (int v : order) if (comp_id[v] == -1) {
            comp.emplace_back();
            dfs2(v, idx++);
        }
    }
    void dfs1(int u) {
        vis[u] = 1;
        for (int v : g[u]) if (!vis[v]) dfs1(v);
        order.push_back(u);
    }
    void dfs2(int u, int id) {
        comp_id[u] = id;
        comp.back().push_back(u);
        for (int v : rg[u]) if (comp_id[v] == -1) dfs2(v, id);
    }
};

struct Dinic {
    struct Edge { int to, rev; ll cap; };
    int n;
    vector<vector<Edge>> g;
    vector<int> lvl, it;
    Dinic(int n) : n(n), g(n), lvl(n), it(n) {}
    void add_edge(int u, int v, ll cap) {
        Edge a{v, (int)g[v].size(), cap};
        Edge b{u, (int)g[u].size(), 0};
        g[u].push_back(a);
        g[v].push_back(b);
    }
    bool bfs(int s, int t) {
        fill(ALL(lvl), -1);
        queue<int> q;
        lvl[s] = 0; q.push(s);
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (auto& e : g[v]) if (e.cap > 0 && lvl[e.to] == -1) {
                lvl[e.to] = lvl[v] + 1;
                q.push(e.to);
            }
        }
        return lvl[t] != -1;
    }
    ll dfs(int v, int t, ll f) {
        if (v == t || f == 0) return f;
        for (int& i = it[v]; i < (int)g[v].size(); i++) {
            Edge& e = g[v][i];
            if (e.cap > 0 && lvl[e.to] == lvl[v] + 1) {
                ll ret = dfs(e.to, t, min(f, e.cap));
                if (ret) {
                    e.cap -= ret;
                    g[e.to][e.rev].cap += ret;
                    return ret;
                }
            }
        }
        return 0;
    }
    ll max_flow(int s, int t) {
        ll flow = 0, pushed;
        while (bfs(s, t)) {
            fill(ALL(it), 0);
            while ((pushed = dfs(s, t, INF64))) flow += pushed;
        }
        return flow;
    }
};

#endif
