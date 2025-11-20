#ifndef CP_STRINGS_HPP
#define CP_STRINGS_HPP

#include "base.hpp"
#include "math.hpp"

inline vector<int> z_function(const string& s) {
    int n = SZ(s);
    vector<int> z(n, 0);
    int l = 0, r = 0;
    for (int i = 1; i < n; i++) {
        if (i <= r) z[i] = min(r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];
        if (i + z[i] - 1 > r) { l = i; r = i + z[i] - 1; }
    }
    return z;
}

inline vector<int> prefix_function(const string& s) {
    int n = SZ(s);
    vector<int> pi(n, 0);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }
    return pi;
}

struct RollingHash {
    static const ull mod = (1ULL << 61) - 1;
    ull base;
    vector<ull> pref, power;
    RollingHash(const string& s, ull base = 911382323) : base(base) {
        int n = SZ(s);
        pref.assign(n + 1, 0);
        power.assign(n + 1, 1);
        for (int i = 0; i < n; i++) {
            pref[i + 1] = add(mul(pref[i], base), s[i]);
            power[i + 1] = mul(power[i], base);
        }
    }
    ull add(ull a, ull b) const {
        ull res = a + b;
        if (res >= mod) res -= mod;
        return res;
    }
    ull mul(ull a, ull b) const {
        ull au = a >> 31, ad = a & ((1ULL << 31) - 1);
        ull bu = b >> 31, bd = b & ((1ULL << 31) - 1);
        ull mid = ad * bu + au * bd;
        ull res = (au * bu << 1) + (mid >> 30) + ((mid & ((1ULL << 30) - 1)) << 31) + ad * bd;
        res = (res >> 61) + (res & mod);
        if (res >= mod) res -= mod;
        return res;
    }
    ull get(int l, int r) const {
        ull res = pref[r] + mod - mul(pref[l], power[r - l]);
        if (res >= mod) res -= mod;
        return res;
    }
};

inline vector<int> manacher(const string& s) {
    int n = SZ(s);
    string t(2 * n + 1, '#');
    REP(i, n) t[2 * i + 1] = s[i];
    int m = SZ(t);
    vector<int> d(m, 0);
    int l = 0, r = -1;
    for (int i = 0; i < m; i++) {
        int k = (i > r) ? 1 : min(d[l + r - i], r - i + 1);
        while (i - k >= 0 && i + k < m && t[i - k] == t[i + k]) k++;
        d[i] = k;
        if (i + k - 1 > r) {
            l = i - k + 1;
            r = i + k - 1;
        }
    }
    return d;
}

inline vector<int> suffix_array(const string& s) {
    string str = s;
    str.push_back(char(0));
    int n = SZ(str);
    vector<int> sa(n), c(n);
    iota(ALL(sa), 0);
    sort(ALL(sa), [&](int a, int b){ return str[a] < str[b]; });
    c[sa[0]] = 0;
    FOR(i,1,n-1) c[sa[i]] = c[sa[i-1]] + (str[sa[i]] != str[sa[i-1]]);
    for (int k = 0; (1 << k) < n; k++) {
        int len = 1 << k;
        vector<int> sa2(n), cnt(n);
        for (int i = 0; i < n; i++) sa2[i] = (sa[i] - len + n) % n;
        for (int x : c) cnt[x]++;
        for (int i = 1; i < n; i++) cnt[i] += cnt[i-1];
        for (int i = n - 1; i >= 0; i--) sa[--cnt[c[sa2[i]]]] = sa2[i];
        vector<int> c2(n);
        c2[sa[0]] = 0;
        for (int i = 1; i < n; i++) {
            pii cur = {c[sa[i]], c[(sa[i] + len) % n]};
            pii prev = {c[sa[i-1]], c[(sa[i-1] + len) % n]};
            c2[sa[i]] = c2[sa[i-1]] + (cur != prev);
        }
        c.swap(c2);
        if (c[sa.back()] == n - 1) break;
    }
    sa.erase(sa.begin());
    return sa;
}

inline vector<int> lcp_array(const string& s, const vector<int>& sa) {
    int n = SZ(s);
    vector<int> rank(n, 0), lcp(n - 1, 0);
    for (int i = 0; i < n; i++) rank[sa[i]] = i;
    int k = 0;
    for (int i = 0; i < n; i++) {
        if (rank[i] == n - 1) { k = 0; continue; }
        int j = sa[rank[i] + 1];
        while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
        lcp[rank[i]] = k;
        if (k) k--;
    }
    return lcp;
}

#endif
