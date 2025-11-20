import math
import random

INF64 = 10 ** 18
MOD = 10 ** 9 + 7
MOD2 = 998244353

def mod_pow(a, e, mod):
    res = 1 % mod
    while e:
        if e & 1:
            res = res * a % mod
        a = a * a % mod
        e >>= 1
    return res

def mod_inv(a, mod):
    return mod_pow(a, mod - 2, mod)

def lcm(a, b):
    return a // math.gcd(a, b) * b

class Comb:
    def __init__(self, n, mod):
        self.mod = mod
        self.fact = [1] * (n + 1)
        for i in range(1, n + 1):
            self.fact[i] = self.fact[i - 1] * i % mod
        self.ifact = [1] * (n + 1)
        self.ifact[n] = mod_inv(self.fact[n], mod)
        for i in range(n, 0, -1):
            self.ifact[i - 1] = self.ifact[i] * i % mod
    def nCr(self, n, r):
        if r < 0 or r > n:
            return 0
        return self.fact[n] * self.ifact[r] % self.mod * self.ifact[n - r] % self.mod

def linear_sieve(n):
    lp = [0] * (n + 1)
    primes = []
    for i in range(2, n + 1):
        if lp[i] == 0:
            lp[i] = i
            primes.append(i)
        for p in primes:
            if p > lp[i] or i * p > n:
                break
            lp[i * p] = p
    return lp, primes

def mul_mod(a, b, mod):
    return (a * b) % mod

def mod_pow_u64(a, e, mod):
    res = 1 % mod
    while e:
        if e & 1:
            res = mul_mod(res, a, mod)
        a = mul_mod(a, a, mod)
        e >>= 1
    return res

def is_probable_prime(n):
    if n < 2:
        return False
    small = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37]
    for p in small:
        if n % p == 0:
            return n == p
    d, s = n - 1, 0
    while d % 2 == 0:
        d //= 2
        s += 1
    for a in [2, 325, 9375, 28178, 450775, 9780504, 1795265022]:
        if a % n == 0:
            continue
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        for _ in range(s - 1):
            x = mul_mod(x, x, n)
            if x == n - 1:
                break
        else:
            return False
    return True

def pollard_rho(n):
    if n % 2 == 0:
        return 2
    if n % 3 == 0:
        return 3
    while True:
        c = random.randrange(1, n)
        x = random.randrange(0, n)
        y = x
        d = 1
        while d == 1:
            x = (mul_mod(x, x, n) + c) % n
            y = (mul_mod(y, y, n) + c) % n
            y = (mul_mod(y, y, n) + c) % n
            d = math.gcd(abs(x - y), n)
        if d != n:
            return d

def factor(n, acc=None):
    if acc is None:
        acc = {}
    if n == 1:
        return acc
    if is_probable_prime(n):
        acc[n] = acc.get(n, 0) + 1
        return acc
    d = pollard_rho(n)
    factor(d, acc)
    factor(n // d, acc)
    return acc

def crt_pair(a, b):
    r1, m1 = a
    r2, m2 = b
    g = math.gcd(m1, m2)
    if (r2 - r1) % g != 0:
        return (0, -1)
    l = m1 // g * m2
    inv = pow(m1 // g, -1, m2 // g)
    x = ((r2 - r1) // g) * inv % (m2 // g)
    res = (r1 + m1 * x) % l
    return (res, l)
