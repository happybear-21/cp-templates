#ifndef CP_BASE_HPP
#define CP_BASE_HPP

#include <algorithm>
#include <array>
#include <bitset>
#include <chrono>
#include <climits>
#include <cmath>
#include <cstdint>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<long long, long long>;

#define FAST_IO ios::sync_with_stdio(false); cin.tie(nullptr)
#define ALL(v) (v).begin(), (v).end()
#define SZ(v) ((int)(v).size())
#define REP(i,n) for (int i = 0; i < (int)(n); ++i)
#define FOR(i,a,b) for (int i = (a); i <= (int)(b); ++i)
#define FORD(i,a,b) for (int i = (a); i >= (int)(b); --i)

const ll INF64 = (1LL << 60);
const int INF32 = (1 << 30);
const int MOD = 1'000'000'007;
const int MOD2 = 998'244'353;

struct RNG {
    mt19937 rng;
    RNG() : rng((uint32_t)chrono::steady_clock::now().time_since_epoch().count()) {}
    int next_int(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
    ll next_ll(ll l, ll r) { return uniform_int_distribution<ll>(l, r)(rng); }
    double next_double() { return uniform_real_distribution<double>(0.0, 1.0)(rng); }
};

#endif
