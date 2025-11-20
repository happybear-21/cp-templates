#ifndef CP_MATH_HPP
#define CP_MATH_HPP

#include "base.hpp"

inline ll mod_pow(ll a, ll e, ll mod) {
    ll res = 1 % mod;
    while (e) {
        if (e & 1) res = (__int128)res * a % mod;
        a = (__int128)a * a % mod;
        e >>= 1;
    }
    return res;
}

inline ll mod_inv(ll a, ll mod) {
    return mod_pow(a, mod - 2, mod);
}

inline ll lcm_ll(ll a, ll b) {
    return a / gcd(a, b) * b;
}

struct Comb {
    int n;
    ll mod;
    vector<ll> fact, ifact;
    Comb(int n, ll mod) : n(n), mod(mod), fact(n + 1), ifact(n + 1) {
        fact[0] = 1;
        FOR(i, 1, n) fact[i] = fact[i - 1] * i % mod;
        ifact[n] = mod_inv(fact[n], mod);
        FORD(i, n, 1) ifact[i - 1] = ifact[i] * i % mod;
    }
    ll nCr(int nn, int r) const {
        if (r < 0 || r > nn) return 0;
        return fact[nn] * ifact[r] % mod * ifact[nn - r] % mod;
    }
};

struct LinearSieve {
    int n;
    vector<int> primes, lp;
    LinearSieve(int n) : n(n), lp(n + 1, 0) {
        primes.reserve(n / 10);
        FOR(i, 2, n) {
            if (!lp[i]) {
                lp[i] = i;
                primes.push_back(i);
            }
            for (int p : primes) {
                if (p > lp[i] || i * 1LL * p > n) break;
                lp[i * p] = p;
            }
        }
    }
    vector<pair<int, int>> factor(int x) const {
        vector<pair<int, int>> res;
        while (x > 1) {
            int p = lp[x], c = 0;
            while (x % p == 0) {
                x /= p;
                ++c;
            }
            res.push_back({p, c});
        }
        return res;
    }
};

inline ull mod_mul64(ull a, ull b, ull mod) {
    return (__int128)a * b % mod;
}

inline ull mod_pow64(ull a, ull e, ull mod) {
    ull res = 1 % mod;
    while (e) {
        if (e & 1) res = mod_mul64(res, a, mod);
        a = mod_mul64(a, a, mod);
        e >>= 1;
    }
    return res;
}

inline ull gcd_u64(ull a, ull b) {
    return b == 0 ? a : gcd_u64(b, a % b);
}

inline bool is_probable_prime(ull n) {
    if (n < 2) return false;
    for (ull p : {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL}) {
        if (n % p == 0) return n == p;
    }
    ull d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    auto witness = [&](ull a) {
        if (a % n == 0) return true;
        ull x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) return true;
        for (ull r = 1; r < s; r++) {
            x = mod_mul64(x, x, n);
            if (x == n - 1) return true;
        }
        return false;
    };
    for (ull a : {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL}) {
        if (a % n == 0) continue;
        if (!witness(a)) return false;
    }
    return true;
}

inline mt19937_64 pollard_rng((uint64_t)chrono::steady_clock::now().time_since_epoch().count());

inline ull pollard_f(ull x, ull c, ull mod) {
    return (mod_mul64(x, x, mod) + c) % mod;
}

inline ull pollard_rho(ull n) {
    if ((n & 1) == 0) return 2;
    if (n % 3 == 0) return 3;
    while (true) {
        ull c = uniform_int_distribution<ull>(1, n - 1)(pollard_rng);
        ull x = uniform_int_distribution<ull>(0, n - 1)(pollard_rng);
        ull y = x;
        ull d = 1;
        while (d == 1) {
            x = pollard_f(x, c, n);
            y = pollard_f(pollard_f(y, c, n), c, n);
            ull diff = x > y ? x - y : y - x;
            d = gcd_u64(diff, n);
        }
        if (d != n) return d;
    }
}

inline void factor_rec(ull n, map<ull, int>& res) {
    if (n == 1) return;
    if (is_probable_prime(n)) { res[n]++; return; }
    ull d = pollard_rho(n);
    factor_rec(d, res);
    factor_rec(n / d, res);
}

inline ll ext_gcd(ll a, ll b, ll& x, ll& y) {
    if (b == 0) { x = 1; y = 0; return a; }
    ll x1, y1;
    ll g = ext_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return g;
}

inline ll mod_inv_any(ll a, ll mod) {
    ll x, y;
    ll g = ext_gcd(a, mod, x, y);
    if (g != 1) return -1;
    x %= mod;
    if (x < 0) x += mod;
    return x;
}

inline pair<ll, ll> crt_pair(pair<ll, ll> a, pair<ll, ll> b) {
    ll r1 = a.first, m1 = a.second;
    ll r2 = b.first, m2 = b.second;
    ll g = gcd(m1, m2);
    if ((r2 - r1) % g != 0) return {0, -1};
    ll l = m1 / g * m2;
    ll inv = mod_inv_any(m1 / g % (m2 / g), m2 / g);
    if (inv == -1) return {0, -1};
    ll x = (__int128)(r2 - r1) / g * inv % (m2 / g);
    ll res = (r1 + (__int128)m1 * x) % l;
    if (res < 0) res += l;
    return {res, l};
}

#endif
