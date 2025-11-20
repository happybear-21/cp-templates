#ifndef CP_DS_HPP
#define CP_DS_HPP

#include "base.hpp"

struct Fenwick {
    int n;
    vector<ll> bit;
    Fenwick(int n) : n(n), bit(n + 1, 0) {}
    void add(int idx, ll val) {
        for (int i = idx + 1; i <= n; i += i & -i) bit[i] += val;
    }
    ll sum_prefix(int idx) const {
        ll res = 0;
        for (int i = idx + 1; i > 0; i -= i & -i) res += bit[i];
        return res;
    }
    ll sum_range(int l, int r) const {
        return sum_prefix(r) - (l ? sum_prefix(l - 1) : 0);
    }
};

struct FenwickRange {
    Fenwick b1, b2;
    FenwickRange(int n) : b1(n), b2(n) {}
    void internal_add(Fenwick& b, int idx, ll val) {
        for (int i = idx + 1; i <= b.n; i += i & -i) b.bit[i] += val;
    }
    ll internal_sum(const Fenwick& b, int idx) const {
        ll res = 0;
        for (int i = idx + 1; i > 0; i -= i & -i) res += b.bit[i];
        return res;
    }
    void range_add(int l, int r, ll val) {
        internal_add(b1, l, val);
        internal_add(b1, r + 1, -val);
        internal_add(b2, l, val * l);
        internal_add(b2, r + 1, -val * (r + 1));
    }
    ll prefix_sum(int idx) const {
        return internal_sum(b1, idx) * (idx + 1) - internal_sum(b2, idx);
    }
    ll range_sum(int l, int r) const {
        return prefix_sum(r) - (l ? prefix_sum(l - 1) : 0);
    }
};

struct DSU {
    vector<int> p, sz;
    DSU(int n = 0) { init(n); }
    void init(int n) {
        p.resize(n);
        sz.assign(n, 1);
        iota(ALL(p), 0);
    }
    int find(int x) {
        while (x != p[x]) x = p[x] = p[p[x]];
        return x;
    }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a;
        sz[a] += sz[b];
        return true;
    }
};

struct RollbackDSU {
    vector<int> p, sz;
    vector<pair<int,int>> st;
    int comps;
    RollbackDSU(int n) : p(n), sz(n, 1), comps(n) { iota(ALL(p), 0); }
    int find(int x) {
        while (x != p[x]) x = p[x];
        return x;
    }
    int snapshot() const { return SZ(st); }
    void rollback(int t) {
        while (SZ(st) > t) {
            auto [b, a] = st.back(); st.pop_back();
            if (b == -1) continue;
            p[b] = b;
            sz[a] -= sz[b];
            comps++;
        }
    }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) { st.push_back({-1, -1}); return false; }
        if (sz[a] < sz[b]) swap(a, b);
        st.push_back({b, a});
        p[b] = a;
        sz[a] += sz[b];
        comps--;
        return true;
    }
};

template <class T, class F>
struct SegmentTree {
    int n;
    F merge;
    T neutral;
    unique_ptr<T[]> st;
    SegmentTree(int n, F merge, T neutral) : n(n), merge(merge), neutral(neutral), st(new T[4 * n]) {}
    void build(const vector<T>& v, int p, int l, int r) {
        if (l == r) { st[p] = v[l]; return; }
        int m = (l + r) >> 1;
        build(v, p << 1, l, m);
        build(v, p << 1 | 1, m + 1, r);
        st[p] = merge(st[p << 1], st[p << 1 | 1]);
    }
    T query(int p, int l, int r, int ql, int qr) const {
        if (qr < l || r < ql) return neutral;
        if (ql <= l && r <= qr) return st[p];
        int m = (l + r) >> 1;
        return merge(query(p << 1, l, m, ql, qr), query(p << 1 | 1, m + 1, r, ql, qr));
    }
    void update(int p, int l, int r, int idx, T val) {
        if (l == r) { st[p] = val; return; }
        int m = (l + r) >> 1;
        if (idx <= m) update(p << 1, l, m, idx, val);
        else update(p << 1 | 1, m + 1, r, idx, val);
        st[p] = merge(st[p << 1], st[p << 1 | 1]);
    }
};

template <class T, class F>
struct LazySegTree {
    int n;
    F merge;
    T neutral, lazy_neutral;
    unique_ptr<T[]> st, lz;
    function<void(T&, T, int, int)> apply;
    LazySegTree(int n, F merge, T neutral, T lazy_neutral, function<void(T&, T, int, int)> apply)
        : n(n), merge(merge), neutral(neutral), lazy_neutral(lazy_neutral), st(new T[4 * n]), lz(new T[4 * n]), apply(apply) {
        fill(lz.get(), lz.get() + 4 * n, lazy_neutral);
    }
    void build(const vector<T>& v, int p, int l, int r) {
        if (l == r) { st[p] = v[l]; return; }
        int m = (l + r) >> 1;
        build(v, p << 1, l, m);
        build(v, p << 1 | 1, m + 1, r);
        st[p] = merge(st[p << 1], st[p << 1 | 1]);
    }
    void push(int p, int l, int r) {
        if (lz[p] == lazy_neutral) return;
        int m = (l + r) >> 1;
        apply(st[p << 1], lz[p], l, m);
        apply(st[p << 1 | 1], lz[p], m + 1, r);
        if (lz[p << 1] == lazy_neutral) lz[p << 1] = lz[p]; else lz[p << 1] += lz[p];
        if (lz[p << 1 | 1] == lazy_neutral) lz[p << 1 | 1] = lz[p]; else lz[p << 1 | 1] += lz[p];
        lz[p] = lazy_neutral;
    }
    void range_update(int p, int l, int r, int ql, int qr, T val) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply(st[p], val, l, r);
            if (lz[p] == lazy_neutral) lz[p] = val; else lz[p] += val;
            return;
        }
        push(p, l, r);
        int m = (l + r) >> 1;
        range_update(p << 1, l, m, ql, qr, val);
        range_update(p << 1 | 1, m + 1, r, ql, qr, val);
        st[p] = merge(st[p << 1], st[p << 1 | 1]);
    }
    T query(int p, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return neutral;
        if (ql <= l && r <= qr) return st[p];
        push(p, l, r);
        int m = (l + r) >> 1;
        return merge(query(p << 1, l, m, ql, qr), query(p << 1 | 1, m + 1, r, ql, qr));
    }
};

template <class T, class F>
struct SparseTable {
    int n, K;
    F op;
    vector<vector<T>> st;
    SparseTable(const vector<T>& v, F op) : n(SZ(v)), op(op) {
        K = 1;
        while ((1 << K) <= n) ++K;
        st.assign(K, vector<T>(n));
        st[0] = v;
        for (int k = 1; k < K; k++) {
            for (int i = 0; i + (1 << k) <= n; i++) {
                st[k][i] = op(st[k - 1][i], st[k - 1][i + (1 << (k - 1))]);
            }
        }
    }
    T query(int l, int r) const {
        int k = 31 - __builtin_clz(r - l + 1);
        return op(st[k][l], st[k][r - (1 << k) + 1]);
    }
};

struct Treap {
    struct Node {
        int key, prior, sz;
        ll add;
        Node *l, *r;
        Node(int key, int prior) : key(key), prior(prior), sz(1), add(0), l(nullptr), r(nullptr) {}
    };
    RNG rng;
    Treap() {}
    int getsz(Node* t) { return t ? t->sz : 0; }
    void push(Node* t) {
        if (t && t->add) {
            if (t->l) { t->l->key += t->add; t->l->add += t->add; }
            if (t->r) { t->r->key += t->add; t->r->add += t->add; }
            t->add = 0;
        }
    }
    void pull(Node* t) {
        if (t) t->sz = 1 + getsz(t->l) + getsz(t->r);
    }
    void split(Node* t, int key, Node*& l, Node*& r) {
        if (!t) { l = r = nullptr; return; }
        push(t);
        if (t->key <= key) {
            split(t->r, key, t->r, r);
            l = t;
        } else {
            split(t->l, key, l, t->l);
            r = t;
        }
        pull(t);
    }
    void merge(Node*& t, Node* l, Node* r) {
        if (!l || !r) t = l ? l : r;
        else if (l->prior > r->prior) {
            push(l);
            merge(l->r, l->r, r);
            t = l;
        } else {
            push(r);
            merge(r->l, l, r->l);
            t = r;
        }
        pull(t);
    }
    void insert(Node*& t, Node* it) {
        if (!t) t = it;
        else if (it->prior > t->prior) {
            split(t, it->key, it->l, it->r);
            t = it;
        } else insert(it->key < t->key ? t->l : t->r, it);
        pull(t);
    }
    void erase(Node*& t, int key) {
        if (!t) return;
        if (t->key == key) {
            Node* tmp = t;
            merge(t, t->l, t->r);
            delete tmp;
        } else erase(key < t->key ? t->l : t->r, key);
        pull(t);
    }
};

struct SqrtDecomp {
    int n, blk;
    vector<ll> arr, bucket;
    SqrtDecomp(const vector<ll>& v) : n(SZ(v)), arr(v) {
        blk = (int)sqrt(n) + 1;
        bucket.assign(blk, 0);
        REP(i, n) bucket[i / blk] += arr[i];
    }
    void update(int idx, ll val) {
        bucket[idx / blk] += val - arr[idx];
        arr[idx] = val;
    }
    ll query(int l, int r) {
        ll res = 0;
        while (l <= r && l % blk) res += arr[l++];
        while (l + blk - 1 <= r) {
            res += bucket[l / blk];
            l += blk;
        }
        while (l <= r) res += arr[l++];
        return res;
    }
};

struct Line {
    ll m, b;
    ll get(ll x) const { return m * x + b; }
};

struct LiChao {
    struct Node {
        Line ln;
        Node* left;
        Node* right;
        Node(Line ln) : ln(ln), left(nullptr), right(nullptr) {}
    };
    ll lo, hi;
    Node* root;
    LiChao(ll lo, ll hi) : lo(lo), hi(hi), root(nullptr) {}
    void add_line(Line nw) { add_line(root, lo, hi, nw); }
    ll query(ll x) const { return query(root, lo, hi, x); }
private:
    void add_line(Node*& node, ll l, ll r, Line nw) {
        if (!node) { node = new Node(nw); return; }
        ll mid = (l + r) >> 1;
        bool lef = nw.get(l) < node->ln.get(l);
        bool m = nw.get(mid) < node->ln.get(mid);
        if (m) swap(nw, node->ln);
        if (l == r) return;
        if (lef != m) add_line(node->left, l, mid, nw);
        else add_line(node->right, mid + 1, r, nw);
    }
    ll query(Node* node, ll l, ll r, ll x) const {
        if (!node) return (ll)4e18;
        ll res = node->ln.get(x);
        if (l == r) return res;
        ll mid = (l + r) >> 1;
        if (x <= mid) res = min(res, query(node->left, l, mid, x));
        else res = min(res, query(node->right, mid + 1, r, x));
        return res;
    }
};

struct HullLine {
    mutable ll k, m, p;
    bool operator<(const HullLine& o) const { return k < o.k; }
    bool operator<(ll x) const { return p < x; }
};

struct LineContainer : multiset<HullLine, less<>> {
    static const ll inf = (1LL << 62);
    ll div_floor(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }
    bool isect(iterator x, iterator y) {
        if (y == end()) { x->p = inf; return false; }
        if (x->k == y->k) x->p = x->m > y->m ? inf : -inf;
        else x->p = div_floor(y->m - x->m, x->k - y->k);
        return x->p >= y->p;
    }
    void add(ll k, ll m) {
        auto z = insert({k, m, 0}), y = z++, x = y;
        while (isect(y, z)) z = erase(z);
        if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
        while ((y = x) != begin() && (--x)->p >= y->p) isect(x, erase(y));
    }
    ll query(ll x) {
        auto l = *lower_bound(x);
        return l.k * x + l.m;
    }
};

#endif
