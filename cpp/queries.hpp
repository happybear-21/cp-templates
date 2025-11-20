#ifndef CP_QUERIES_HPP
#define CP_QUERIES_HPP

#include "base.hpp"

struct Mo {
    struct Query { int l, r, idx; };
    int n, blk;
    vector<ll> ans;
    vector<int> cnt;
    ll cur = 0;
    Mo(int n, int q) : n(n) {
        blk = max(1, (int) sqrt(n));
        ans.assign(q, 0);
        cnt.assign(1'000'001, 0);
    }
    void add_left(int idx, const vector<int>& a) {
        int x = a[idx];
        cur -= 1LL * cnt[x] * cnt[x] * x;
        cnt[x]++;
        cur += 1LL * cnt[x] * cnt[x] * x;
    }
    void add_right(int idx, const vector<int>& a) { add_left(idx, a); }
    void remove_left(int idx, const vector<int>& a) {
        int x = a[idx];
        cur -= 1LL * cnt[x] * cnt[x] * x;
        cnt[x]--;
        cur += 1LL * cnt[x] * cnt[x] * x;
    }
    void remove_right(int idx, const vector<int>& a) { remove_left(idx, a); }
    void process(vector<Query>& qs, const vector<int>& a) {
        sort(ALL(qs), [&](const Query& A, const Query& B) {
            int blockA = A.l / blk, blockB = B.l / blk;
            if (blockA != blockB) return blockA < blockB;
            if (blockA & 1) return A.r > B.r;
            return A.r < B.r;
        });
        int L = 0, R = -1;
        for (auto q : qs) {
            while (L > q.l) add_left(--L, a);
            while (R < q.r) add_right(++R, a);
            while (L < q.l) remove_left(L++, a);
            while (R > q.r) remove_right(R--, a);
            ans[q.idx] = cur;
        }
    }
};

struct TreeMo {
    int n, timer, blk;
    vector<int> st, en, euler;
    vector<int> vis;
    vector<ll> ans;
    vector<int> freq;
    ll cur = 0;
    TreeMo(int n) : n(n), st(n), en(n), vis(n, 0) {}
    void dfs(int u, int p, const vector<vector<int>>& g) {
        st[u] = SZ(euler);
        euler.push_back(u);
        for (int v : g[u]) if (v != p) dfs(v, u, g);
        en[u] = SZ(euler);
    }
    struct Query { int l, r, idx; };
    void process(const vector<vector<int>>& g, const vector<int>& val, vector<Query>& qs) {
        dfs(0, -1, g);
        blk = max(1, (int)sqrt(SZ(euler)));
        sort(ALL(qs), [&](const Query& A, const Query& B) {
            int blockA = A.l / blk, blockB = B.l / blk;
            if (blockA != blockB) return blockA < blockB;
            if (blockA & 1) return A.r > B.r;
            return A.r < B.r;
        });
        freq.assign(1'000'001, 0);
        ans.assign(SZ(qs), 0);
        int L = 0, R = -1;
        auto toggle = [&](int node) {
            int x = val[node];
            if (vis[node]) {
                cur -= 1LL * freq[x] * freq[x] * x;
                freq[x]--;
                cur += 1LL * freq[x] * freq[x] * x;
            } else {
                cur -= 1LL * freq[x] * freq[x] * x;
                freq[x]++;
                cur += 1LL * freq[x] * freq[x] * x;
            }
            vis[node] ^= 1;
        };
        for (auto q : qs) {
            while (L > q.l) toggle(euler[--L]);
            while (R < q.r) toggle(euler[++R]);
            while (L < q.l) toggle(euler[L++]);
            while (R > q.r) toggle(euler[R--]);
            ans[q.idx] = cur;
        }
    }
};

#endif
