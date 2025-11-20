#ifndef CP_DP_HPP
#define CP_DP_HPP

#include "base.hpp"

inline void divide_conquer_dp(int l, int r, int optl, int optr, vector<ll>& cur, const vector<ll>& prev, function<ll(int,int)> cost) {
    if (l > r) return;
    int mid = (l + r) >> 1;
    pair<ll, int> best = {LLONG_MAX, -1};
    int start = optl, end = min(mid, optr);
    for (int k = start; k <= end; k++) {
        ll cand = prev[k] + cost(k, mid);
        if (cand < best.first) best = {cand, k};
    }
    cur[mid] = best.first;
    int opt = best.second;
    divide_conquer_dp(l, mid - 1, optl, opt, cur, prev, cost);
    divide_conquer_dp(mid + 1, r, opt, optr, cur, prev, cost);
}

#endif
