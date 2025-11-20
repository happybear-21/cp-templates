from collections import defaultdict

def z_function(s):
    n = len(s)
    z = [0] * n
    l = r = 0
    for i in range(1, n):
        if i <= r:
            z[i] = min(r - i + 1, z[i - l])
        while i + z[i] < n and s[z[i]] == s[i + z[i]]:
            z[i] += 1
        if i + z[i] - 1 > r:
            l, r = i, i + z[i] - 1
    return z

def prefix_function(s):
    n = len(s)
    pi = [0] * n
    for i in range(1, n):
        j = pi[i - 1]
        while j > 0 and s[i] != s[j]:
            j = pi[j - 1]
        if s[i] == s[j]:
            j += 1
        pi[i] = j
    return pi

class RollingHash:
    MOD = (1 << 61) - 1
    def __init__(self, s, base=911382323):
        self.base = base
        n = len(s)
        self.pref = [0] * (n + 1)
        self.power = [1] * (n + 1)
        for i, ch in enumerate(s):
            self.pref[i + 1] = self._add(self._mul(self.pref[i], base), ord(ch))
            self.power[i + 1] = self._mul(self.power[i], base)
    def _add(self, a, b):
        res = a + b
        if res >= self.MOD:
            res -= self.MOD
        return res
    def _mul(self, a, b):
        au, ad = a >> 31, a & ((1 << 31) - 1)
        bu, bd = b >> 31, b & ((1 << 31) - 1)
        mid = ad * bu + au * bd
        res = (au * bu << 1) + (mid >> 30) + ((mid & ((1 << 30) - 1)) << 31) + ad * bd
        res = (res >> 61) + (res & self.MOD)
        if res >= self.MOD:
            res -= self.MOD
        return res
    def get(self, l, r):
        res = self.pref[r] + self.MOD - self._mul(self.pref[l], self.power[r - l])
        if res >= self.MOD:
            res -= self.MOD
        return res

def manacher(s):
    t = ['#']
    for ch in s:
        t.append(ch)
        t.append('#')
    n = len(t)
    d = [0] * n
    l = r = 0
    for i in range(n):
        k = 1 if i > r else min(d[l + r - i], r - i + 1)
        while i - k >= 0 and i + k < n and t[i - k] == t[i + k]:
            k += 1
        d[i] = k
        if i + k - 1 > r:
            l, r = i - k + 1, i + k - 1
    return d

def suffix_array(s):
    s += '\0'
    n = len(s)
    sa = list(range(n))
    sa.sort(key=lambda i: s[i])
    c = [0] * n
    for i in range(1, n):
        c[sa[i]] = c[sa[i - 1]] + (s[sa[i]] != s[sa[i - 1]])
    k = 0
    while (1 << k) < n:
        len2 = 1 << k
        sa = [(sa[i] - len2) % n for i in range(n)]
        cnt = [0] * n
        for x in c:
            cnt[x] += 1
        for i in range(1, n):
            cnt[i] += cnt[i - 1]
        sa2 = [0] * n
        for i in range(n - 1, -1, -1):
            cnt[c[sa[i]]] -= 1
            sa2[cnt[c[sa[i]]]] = sa[i]
        sa = sa2
        c2 = [0] * n
        for i in range(1, n):
            cur = (c[sa[i]], c[(sa[i] + len2) % n])
            prev = (c[sa[i - 1]], c[(sa[i - 1] + len2) % n])
            c2[sa[i]] = c2[sa[i - 1]] + (cur != prev)
        c = c2
        if c[sa[-1]] == n - 1:
            break
        k += 1
    sa = sa[1:]
    return sa

def lcp_array(s, sa):
    n = len(s)
    rank = [0] * n
    for i, pos in enumerate(sa):
        rank[pos] = i
    lcp = [0] * (n - 1)
    k = 0
    for i in range(n):
        if rank[i] == n - 1:
            k = 0
            continue
        j = sa[rank[i] + 1]
        while i + k < n and j + k < n and s[i + k] == s[j + k]:
            k += 1
        lcp[rank[i]] = k
        if k:
            k -= 1
    return lcp
